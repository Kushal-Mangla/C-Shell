// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include<time.h>
#include<dirent.h>
#include<sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include"ctype.h"

#define MAX_ARGS 64
#define LOG_SIZE 15
extern char *args[MAX_ARGS];
extern char *arg1[MAX_ARGS];
extern char* home_directory;
extern char* prev_directory;
extern char* current_directory;
extern char* file_directory ;
extern char* background_directory ;
extern char command_log[LOG_SIZE][1024];
extern int log_count ;
extern int latest_index; ;
extern char* username;
extern int file_count;
extern int dir_count;
extern char* file_to_print ;
extern char* dir_to_change ;
extern char* time_stamp;

void replace_tabs_with_spaces(char *str);
int tokenize_input(char *input);
void working_directory();
void trim_whitespace(char *str);

#endif // GLOBALS_H