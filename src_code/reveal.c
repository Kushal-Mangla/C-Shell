#include"reveal.h"
#include"print.h"

extern int alphasort(const struct dirent **a, const struct dirent **b);

int case_insensitive_compare(const struct dirent **a, const struct dirent **b) {
    return strcasecmp((*a)->d_name, (*b)->d_name);
}

void print_file_details(const struct stat *file_stat, const char *filename) 
{
    char permissions[11];
    struct passwd *pw = getpwuid(file_stat->st_uid);
    struct group *gr = getgrgid(file_stat->st_gid);
    char timebuf[80];
    struct tm *timeinfo;
    // File type and permissions
    permissions[0] = S_ISDIR(file_stat->st_mode) ? 'd' : '-';
    permissions[1] = (file_stat->st_mode & S_IRUSR) ? 'r' : '-';
    permissions[2] = (file_stat->st_mode & S_IWUSR) ? 'w' : '-';
    permissions[3] = (file_stat->st_mode & S_IXUSR) ? 'x' : '-';
    permissions[4] = (file_stat->st_mode & S_IRGRP) ? 'r' : '-';
    permissions[5] = (file_stat->st_mode & S_IWGRP) ? 'w' : '-';
    permissions[6] = (file_stat->st_mode & S_IXGRP) ? 'x' : '-';
    permissions[7] = (file_stat->st_mode & S_IROTH) ? 'r' : '-';
    permissions[8] = (file_stat->st_mode & S_IWOTH) ? 'w' : '-';
    permissions[9] = (file_stat->st_mode & S_IXOTH) ? 'x' : '-';
    permissions[10] = '\0';

    // Modification time
    timeinfo = localtime(&file_stat->st_mtime);
    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", timeinfo);

    // Print the file details
    printf("%s %2ld %s %s %10ld %s ",
           permissions,
           file_stat->st_nlink,
           pw->pw_name,
           gr->gr_name,
           file_stat->st_size,
           timebuf);

    if(filename[0] == '.')
    {
        // hidden file
        printf(COLOR_YELLOW "%s" COLOR_RESET "\n",filename);
    }
    else if(S_ISDIR(file_stat->st_mode))
    {
        // directory
        printf(COLOR_BLUE "%s" COLOR_RESET "\n", filename);
    }
    else if((file_stat->st_mode & S_IXUSR) || (file_stat->st_mode & S_IXGRP) || (file_stat->st_mode & S_IXOTH))
    {
        // executable file
        printf(COLOR_GREEN "%s" COLOR_RESET "\n", filename);
    }
    else if(S_ISREG(file_stat->st_mode))
    {
        // regular file
        printf(COLOR_WHITE "%s" COLOR_RESET "\n", filename);
    }
    else
    {
        // other types of files
        printf("%s\n", filename);
    }
}
void print_file(const struct stat *file_stat, const char *filename) 
{
    if(filename[0] == '.')
    {
        // hidden file
        printf(COLOR_YELLOW "%s" COLOR_RESET "\n",filename);
    }
    else if(S_ISDIR(file_stat->st_mode))
    {
        // directory
        printf(COLOR_BLUE "%s" COLOR_RESET "\n", filename);
    }
    else if((file_stat->st_mode & S_IXUSR) || (file_stat->st_mode & S_IXGRP) || (file_stat->st_mode & S_IXOTH))
    {
        // executable file
        printf(COLOR_GREEN "%s" COLOR_RESET "\n", filename);
    }
    else if(S_ISREG(file_stat->st_mode))
    {
        // regular file
        printf(COLOR_WHITE "%s" COLOR_RESET "\n", filename);
    }
    else
    {
        // other types of files
        printf("%s\n", filename);
    }
}
void list_directory(const char *directory, int show_hidden, int detailed) {
    struct dirent **namelist;
    int n;
    int flag = 0;
    long long int total_size = 0;
    long long int extra_size = 0;
    // Scan the directory
    // n = scandir(directory, &namelist, NULL, alphasort);
    n = scandir(directory, &namelist, NULL, case_insensitive_compare);

    if (n < 0) {
        perror("scandir");
        return;
    }
    for (int i = 0; i < n; i++) {
        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s", directory, namelist[i]->d_name);

        struct stat file_stat;
        if (stat(filepath, &file_stat) == -1) {
            perror("stat");
            continue;
        }

        // Accumulate the size of regular files
        
        total_size += file_stat.st_blocks;
        if (!show_hidden && namelist[i]->d_name[0] == '.') {
            extra_size += file_stat.st_blocks ;
        }
    }
    if(show_hidden)
    {
        printf("total %lld\n", total_size/2);
    }
        
    else
    {
        printf("total %lld\n" , (total_size - extra_size)/2);
    }

    // Iterate through the directory entries
    for (int i = 0; i < n; i++) {
        // Skip hidden files if show_hidden is not set
        if (!show_hidden && namelist[i]->d_name[0] == '.') {
            free(namelist[i]);
            continue;
        }

        struct stat file_stat;
        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s", directory, namelist[i]->d_name);

        // Print detailed information if detailed flag is set
        if (detailed) {
            if (stat(filepath, &file_stat) == 0) {
                print_file_details(&file_stat, namelist[i]->d_name);
            } else {
                perror("stat");
            }
        } 
        else 
        {
            // Print the directory entry name
            flag=1;
            if (stat(filepath, &file_stat) == 0) {
                print_file(&file_stat, namelist[i]->d_name);
            } 
            else {
                perror("stat");
            }
            // printf(COLOR_BLUE "%s    ", namelist[i]->d_name);
        }
        free(namelist[i]);
    }
    free(namelist);
}

void reveal_implementation()
{
    int show_hidden = 0;
    int detailed = 0;
    char directory[4096] = "";

    // Parse flags and directory
    for (int i = 1; args[i] != NULL; i++) 
    {
        if (strcmp(args[i], "-a") == 0) 
        {
            show_hidden = 1;
        } 
        else if (strcmp(args[i], "-l") == 0) 
        {
            detailed = 1;
        }
        else if(strcmp(args[i], "-la") == 0 || strcmp(args[i], "-al") == 0)
        {
            show_hidden = 1;
            detailed = 1;
        }
        else if(args[i][0] == '-')
        {
            int l1 = strlen(args[i]);
            for(int j=1;j<l1;j++)
            {
                if(args[i][j] == 'a')
                {
                    show_hidden = 1;
                }
                else if(args[i][j] == 'l')
                {
                    detailed = 1;
                }
                else
                {
                    fprintf(stderr, "ls: invalid option -- '%c'\n", args[i][j]);
                }
            }
        }
        else if(args[i][0] == '~')
        {
            char path[4096];
            if (home_directory == NULL) {
                fprintf(stderr, "Home directory not set.\n");
            }
            sprintf(path, "%s%s", home_directory, args[i] + 1);
            strcpy(directory, path);
        }
        else if(strcmp(args[i], "..")==0)
        {
            char cwd[4096];
            if (getcwd(cwd, sizeof(cwd)) == NULL) {
                perror("getcwd() error");
            }
            char prev_1[4096];
            strcpy(prev_1 , cwd);
            chdir("..");
            if (getcwd(cwd, sizeof(cwd)) == NULL) {
                perror("getcwd() error");
            }
            strcpy(directory, cwd);
            chdir(prev_1);
        }
        else if(strcmp(args[1],"~")== 0)
        {
            strcpy(directory , home_directory);
        }
        else if(strcmp(args[i] , "-") == 0)
        {
            strcpy(directory , prev_directory);
        }
        else
        {    
            strcpy(directory, args[i]);
        }
    }
    char cwd[4096];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
    }
    if (strcmp(directory, "") == 0) {
        printf("%s\n", cwd);
        list_directory(cwd, show_hidden, detailed);
    } 
    else 
    {
        list_directory(directory, show_hidden, detailed);
    }
}
