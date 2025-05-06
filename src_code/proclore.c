#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

void proclore(int pid) {
    char path[256];
    char buffer[256];
    FILE *file;

    // If pid is 0, use the current shell's PID
    if (pid == 0) {
        pid = getpid();
    }

    // Print PID
    printf("PID: %d\n", pid);

    // Read /proc/[pid]/status
    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    file = fopen(path, "r");
    if (!file) {
        perror("fopen");
        return;
    }

    char state[32];
    int pgrp;
    unsigned long vsize;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (strncmp(buffer, "State:", 6) == 0) {
            sscanf(buffer, "State:\t%s", state);
        } else if (strncmp(buffer, "Tgid:", 5) == 0) {
            sscanf(buffer, "Tgid:\t%d", &pgrp);
        } else if (strncmp(buffer, "VmSize:", 7) == 0) {
            sscanf(buffer, "VmSize:\t%lu", &vsize);
        }
    }
    int group_id = getpgid(pid);
    fclose(file);
    if (pid == group_id) {
        strcat(state, "+");
    }
    // Print process status, process group, and virtual memory size
    printf("Process Status: %s\n", state);
    printf("Process Group: %d\n", group_id);
    printf("Virtual Memory: %lu kB\n", vsize);

    // Read /proc/[pid]/exe to get the executable path
    snprintf(path, sizeof(path), "/proc/%d/exe", pid);
    ssize_t len = readlink(path, buffer, sizeof(buffer) - 1);
    if (len != -1) {
        buffer[len] = '\0';
        printf("Executable Path: %s\n", buffer);
    } else {
        perror("readlink");
    }
}
