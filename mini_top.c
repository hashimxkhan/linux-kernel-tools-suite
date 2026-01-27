#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Read total CPU time from /proc/stat (sum of all jiffies) */
unsigned long long read_total_cpu_time(void) {
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) {
        perror("fopen /proc/stat");
        exit(1);
    }

    char line[512];
    fgets(line, sizeof(line), fp);
    fclose(fp);

    unsigned long long user, nice, system, idle, iowait, irq, softirq;
    sscanf(line, "cpu %llu %llu %llu %llu %llu %llu %llu",
           &user, &nice, &system, &idle, &iowait, &irq, &softirq);

    return user + nice + system + idle + iowait + irq + softirq;
}

/* Read process CPU time (utime + stime) from /proc/<pid>/stat */
unsigned long long read_process_cpu_time(pid_t pid) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);

    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("fopen process stat");
        exit(1);
    }

    char line[512];
    fgets(line, sizeof(line), fp);
    fclose(fp);
    char *rparen = strrchr(line, ')');
    if (!rparen) {
        fprintf(stderr, "stat parse error\n");
        exit(1);
    }
    unsigned long long utime, stime;
    sscanf(rparen + 2,
           "%*c %*d %*d %*d %*d %*u %*u %*u %*u %*u "
           "%llu %llu",
           &utime, &stime);

    return utime + stime;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <pid>\n", argv[0]);
        return 1;
    }

    pid_t pid = atoi(argv[1]);

    unsigned long long total1 = read_total_cpu_time();
    unsigned long long proc1  = read_process_cpu_time(pid);

    sleep(1);

    unsigned long long total2 = read_total_cpu_time();
    unsigned long long proc2  = read_process_cpu_time(pid);

    unsigned long long total_delta = total2 - total1;
    unsigned long long proc_delta  = proc2 - proc1;

    double cpu = 0.0;
    if (total_delta > 0)
        cpu = (double)proc_delta / (double)total_delta * 100.0;

    printf("PID %d CPU: %.2f%%\n", pid, cpu);
    return 0;
}

