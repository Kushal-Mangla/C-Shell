#ifndef SEEK_H
#define SEEK_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

void seek(const char *target_name, const char *target_dir, int only_files, int only_dirs, int execute_flag);
void print_with_color(const char *path, int is_dir);

#endif // SEEK_H