// globals.c
#include "globals.h"

char* home_directory = " ";
char* prev_directory = "";
char* current_directory;
char *args[MAX_ARGS];
char *arg1[MAX_ARGS];
char *file_directory = "";
char *background_directory = "";
char command_log[LOG_SIZE][1024];
int log_count = 0;
int latest_index = -1;
char *username;
int file_count = 0;
int dir_count = 0;
char *file_to_print = "";
char *dir_to_change = "";
char *time_stamp = "";

void working_directory() {
    char cwd[4096];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
    }
    printf("%s\n", cwd);
}

void trim_whitespace(char *str) {
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0) return; // All spaces?

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator
    *(end + 1) = 0;
}