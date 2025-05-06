#ifndef LOG_H
#define LOG_H

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

// #define LOG_SIZE 15

void load_log();
void save_log();
void add_to_log(const char *command);
void display_log();
void clear_file();
void execute_from_log(int index);

#endif // LOG_H