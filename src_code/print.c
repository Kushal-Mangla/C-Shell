#include"print.h"
#include"globals.h"

#define COLOR_RESET   "\x1b[0m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_WHITE   "\x1b[37m"

#define PATH_MAX 4096

void print_direc() {
    char cwd[PATH_MAX];
    struct utsname sys_info;
    // Get the username
    username = getlogin();   
    // Get the system name
    if (uname(&sys_info) == -1) {
        perror("uname() error");
    }
    
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
    }
    if (strncmp(cwd, home_directory, strlen(home_directory)) == 0) {
        // Replace home_dir part with ~
        printf(COLOR_GREEN "%s@%s" COLOR_RESET ":" COLOR_RED "~%s" COLOR_RESET "> ", username, sys_info.nodename, cwd + strlen(home_directory));
        
    } else {
        printf(COLOR_GREEN "%s@%s" COLOR_RESET ":" COLOR_RED "%s" COLOR_RESET "> ", username, sys_info.nodename, cwd);
    }
    
}
