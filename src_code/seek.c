#include"seek.h"
#include"globals.h"

void get_base_name(const char *filename, char *base_name) {
    char *dot = strrchr(filename, '.');
    if (dot) {
        strncpy(base_name, filename, dot - filename);
        base_name[dot - filename] = '\0';
    } else {
        strcpy(base_name, filename);
    }
}

void print_with_color(const char *path, int is_dir) {
    char display_path[1024];
    if (is_dir) {
        if (strncmp(path, home_directory, strlen(home_directory)) == 0) 
        {
            snprintf(display_path, sizeof(display_path), "~%s", path + strlen(home_directory));
            printf("\033[34m%s\033[0m\n", display_path);  // Blue for directories
        }
        else
        {
            printf("\033[34m%s\033[0m\n", path);  // Blue for directories
        }
    } 
    else 
    {
        if (strncmp(path, home_directory, strlen(home_directory)) == 0) 
        {
            snprintf(display_path, sizeof(display_path), "~%s", path + strlen(home_directory));
            printf("\033[32m%s\033[0m\n", display_path);  // Green for files
        }
        else
        {
            printf("\033[32m%s\033[0m\n", path);  // Green for files
        }
    }
}

void seek(const char *target_name, const char *target_dir, int only_dirs, int only_files, int execute_flag) 
{
    DIR *dir;
    struct dirent *entry;
    struct stat entry_stat;
    char new_dir[1024];
    if(target_dir[0] == '.')
    {
        target_dir += 1;
        // memmove(target_dir, target_dir+1, strlen(target_dir));
    }
    if(target_dir[0] == '~')
    {
        snprintf(new_dir, sizeof(new_dir), "%s%s", home_directory, target_dir + 1);
        target_dir = new_dir;
    }
    if(strcmp(target_dir , "-") == 0)
    {
        target_dir = prev_directory;
    }
    if(strcmp(target_dir , "..") == 0)
    {
        char cwd[4096];
        char directory[4096];
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
        target_dir = directory ;
    }
    if (!(dir = opendir(target_dir))) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", target_dir, entry->d_name);

        // Ignore '.' and '..'
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (stat(path, &entry_stat) == -1) {
            // perror("stat");
            continue;
        }
        // char base_name[256];
        // get_base_name(entry->d_name, base_name);
        char base_name[256];
        //get_base_name(entry->d_name, base_name);

        if (S_ISDIR(entry_stat.st_mode)) {
            if (!only_files && strncmp(entry->d_name, target_name,strlen(target_name)) == 0) 
            {   
                dir_count++;
                strcpy(dir_to_change, path);
                print_with_color(path, 1);
            }
            // Recursively search in subdirectories
            seek(target_name, path, only_dirs, only_files, execute_flag);
        } 
        else if (S_ISREG(entry_stat.st_mode)) 
        {
            if (!only_dirs && strncmp(entry->d_name, target_name,strlen(target_name)) == 0) 
            {
                file_count++;
                strcpy(file_to_print, path);
                print_with_color(path, 0);
            }
        }
    }
    closedir(dir);
}