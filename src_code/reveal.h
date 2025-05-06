#ifndef GREVEAL_H
#define REVEAL_H

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
#include"globals.h"

void print_file_details(const struct stat *file_stat, const char *filename);

void list_directory(const char *directory, int show_hidden, int detailed);

void reveal_implementation();
#endif