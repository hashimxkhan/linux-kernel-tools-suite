#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * syscall_snoop — peek at what syscall a process is currently blocked in.
 *
 * Reads /proc/<pid>/syscall which exposes the syscall number and arguments
 * of a process that is currently inside a system call, without needing
 * ptrace or any attachment.
 *
 * Kernel interface: /proc/<pid>/syscall
 */

/* x86_64 syscall number → name mapping (common subset) */
struct syscall_entry {
    int nr;
    const char *name;
};

static const struct syscall_entry syscall_table[] = {
    {0,   "read"},
    {1,   "write"},
    {2,   "open"},
    {3,   "close"},
    {4,   "stat"},
    {5,   "fstat"},
    {6,   "lstat"},
    {7,   "poll"},
    {8,   "lseek"},
    {9,   "mmap"},
    {10,  "mprotect"},
    {11,  "munmap"},
    {16,  "ioctl"},
    {17,  "pread64"},
    {18,  "pwrite64"},
    {19,  "readv"},
    {20,  "writev"},
    {21,  "access"},
    {22,  "pipe"},
    {23,  "select"},
    {24,  "sched_yield"},
    {32,  "dup"},
    {33,  "dup2"},
    {35,  "nanosleep"},
    {39,  "getpid"},
    {41,  "socket"},
    {42,  "connect"},
    {43,  "accept"},
    {44,  "sendto"},
    {45,  "recvfrom"},
    {46,  "sendmsg"},
    {47,  "recvmsg"},
    {48,  "shutdown"},
    {49,  "bind"},
    {50,  "listen"},
    {56,  "clone"},
    {57,  "fork"},
    {58,  "vfork"},
    {59,  "execve"},
    {60,  "exit"},
    {61,  "wait4"},
    {62,  "kill"},
    {72,  "fcntl"},
    {73,  "flock"},
    {78,  "getdents"},
    {79,  "getcwd"},
    {80,  "chdir"},
    {82,  "rename"},
    {83,  "mkdir"},
    {84,  "rmdir"},
    {87,  "unlink"},
    {89,  "readlink"},
    {102, "getuid"},
    {110, "getppid"},
    {157, "prctl"},
    {186, "gettid"},
    {202, "futex"},
    {217, "getdents64"},
    {228, "clock_gettime"},
    {230, "clock_nanosleep"},
    {231, "exit_group"},
    {232, "epoll_wait"},
    {257, "openat"},
    {262, "newfstatat"},
    {270, "pselect6"},
    {271, "ppoll"},
    {280, "timerfd_settime"},
    {281, "timerfd_gettime"},
    {282, "accept4"},
    {288, "accept4"},
    {289, "signalfd4"},
    {290, "eventfd2"},
    {291, "epoll_create1"},
    {292, "dup3"},
    {293, "pipe2"},
    {302, "prlimit64"},
    {318, "getrandom"},
    {-1,  NULL}
};

const char *syscall_name(int nr) {
    for (int i = 0; syscall_table[i].name != NULL; i++) {
        if (syscall_table[i].nr == nr)
            return syscall_table[i].name;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <pid>\n", argv[0]);
        return 1;
    }

    int pid = atoi(argv[1]);
    if (pid <= 0) {
        fprintf(stderr, "invalid pid: %s\n", argv[1]);
        return 1;
    }

    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/syscall", pid);

    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("fopen");
        fprintf(stderr, "hint: /proc/<pid>/syscall may require root or same-user access\n");
        return 1;
    }

    char line[512];
    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        fprintf(stderr, "could not read syscall info for PID %d\n", pid);
        return 1;
    }
    fclose(fp);

    /* Remove trailing newline */
    line[strcspn(line, "\n")] = '\0';

    /* "running" means the process is on-CPU, not in a syscall */
    if (strcmp(line, "running") == 0) {
        printf("PID %d: running (not in a syscall)\n", pid);
        return 0;
    }

    /* Format: syscall_nr arg0 arg1 arg2 arg3 arg4 arg5 sp pc */
    long long nr;
    unsigned long long args[6], sp, pc;

    int parsed = sscanf(line, "%lld %llx %llx %llx %llx %llx %llx %llx %llx",
                         &nr, &args[0], &args[1], &args[2],
                         &args[3], &args[4], &args[5], &sp, &pc);

    if (parsed < 1) {
        fprintf(stderr, "unexpected format: %s\n", line);
        return 1;
    }

    const char *name = syscall_name((int)nr);

    printf("PID %d blocked in syscall:\n", pid);
    if (name)
        printf("  syscall: %s (%lld)\n", name, nr);
    else
        printf("  syscall: %lld (unknown)\n", nr);

    if (parsed >= 7) {
        printf("  args:    ");
        for (int i = 0; i < 6; i++) {
            printf("0x%llx", args[i]);
            if (i < 5) printf(", ");
        }
        printf("\n");
    }

    if (parsed >= 9) {
        printf("  sp:      0x%llx\n", sp);
        printf("  pc:      0x%llx\n", pc);
    }

    return 0;
}
