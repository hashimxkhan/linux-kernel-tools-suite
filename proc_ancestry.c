#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * proc_ancestry — trace a process's parent chain up to PID 1 (init).
 *
 * Reads /proc/<pid>/stat to extract ppid and process name,
 * then walks upward printing the full lineage.
 *
 * Kernel interface: /proc/<pid>/stat
 */

struct proc_info {
    int pid;
    int ppid;
    char name[256];
    char state;
};

/* Parse /proc/<pid>/stat and fill out a proc_info struct.
 * Returns 0 on success, -1 on failure. */
int read_proc_info(int pid, struct proc_info *info) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);

    FILE *fp = fopen(path, "r");
    if (!fp)
        return -1;

    char line[512];
    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        return -1;
    }
    fclose(fp);

    char *lparen = strchr(line, '(');
    char *rparen = strrchr(line, ')');
    if (!lparen || !rparen)
        return -1;

    /* Extract name between parentheses */
    size_t len = rparen - lparen - 1;
    if (len >= sizeof(info->name))
        len = sizeof(info->name) - 1;
    strncpy(info->name, lparen + 1, len);
    info->name[len] = '\0';

    /* Fields after ')': state ppid ... */
    char state;
    int ppid;
    if (sscanf(rparen + 2, "%c %d", &state, &ppid) != 2)
        return -1;

    info->pid = pid;
    info->ppid = ppid;
    info->state = state;

    return 0;
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

    /* Collect ancestry into a stack so we can print top-down */
    struct proc_info chain[256];
    int depth = 0;

    int current = pid;
    while (current > 0 && depth < 256) {
        if (read_proc_info(current, &chain[depth]) != 0) {
            fprintf(stderr, "cannot read /proc/%d/stat (process gone?)\n", current);
            break;
        }
        current = chain[depth].ppid;
        depth++;

        /* PID 1 has ppid 0 — stop after recording it */
        if (chain[depth - 1].pid == 1)
            break;
    }

    if (depth == 0) {
        fprintf(stderr, "no process info found\n");
        return 1;
    }

    /* Print top-down: init → ... → target */
    printf("ANCESTRY of PID %d (%d levels deep)\n\n", pid, depth);

    for (int i = depth - 1; i >= 0; i--) {
        /* Indent to show depth */
        for (int j = 0; j < (depth - 1 - i); j++)
            printf("  ");

        if (i == depth - 1)
            printf("[%d] %s (%c)\n", chain[i].pid, chain[i].name, chain[i].state);
        else
            printf("└─ [%d] %s (%c)\n", chain[i].pid, chain[i].name, chain[i].state);
    }

    return 0;
}
