#ifndef HOP_H
#define HOP_H
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

void update_prev_dir() ;

void hop_implementation();

#endif