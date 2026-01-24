#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>

int main(void) {
    DIR *proc = opendir("/proc");
    if (!proc) {
        perror("opendir");
        return 1;
    }
    printf("%-6s %-4s %s\n", "PID", "ST", "NAME");
    struct dirent *entry;
    while ((entry = readdir(proc)) != NULL) {
        if (!isdigit(entry->d_name[0])) {
            continue;
	}
        char path[256];
        snprintf(path, sizeof(path), "/proc/%s/stat", entry->d_name);
        FILE *fp = fopen(path, "r");
        if (!fp) {
            continue;
	}
        char line[512];
        if (!fgets(line, sizeof(line), fp)) {
            fclose(fp);
            continue;
        }
        fclose(fp);
        char *lparen = strchr(line, '(');
        char *rparen = strrchr(line, ')');
        if (!lparen || !rparen)
            continue;

        char name[256];
        size_t len = rparen - lparen - 1;
        if (len >= sizeof(name))
            len = sizeof(name) - 1;

        strncpy(name, lparen + 1, len);
        name[len] = '\0';

        char state = *(rparen + 2);

        printf("%-6s %-4c %s\n", entry->d_name, state, name);
    }

    closedir(proc);
    return 0;
}

