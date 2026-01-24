#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>

int main(void) {
    DIR *proc = opendir("/proc");
    if (!proc) {
        perror("opendir");
        return 1;
    }
    struct dirent *entry;
    while ((entry = readdir(proc)) != NULL) {
        if (isdigit(entry->d_name[0])) {
            printf("%s\n", entry->d_name);
        }
    }
    closedir(proc);
    return 0;
}
