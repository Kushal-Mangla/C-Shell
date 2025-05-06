#include"hop.h"

void update_prev_dir() 
{
    char prev_dir[4096];
    if (getcwd(prev_dir, sizeof(prev_dir)) == NULL) {
        perror("getcwd() error");
    }
    prev_directory = malloc(strlen(prev_dir) + 1);
    strcpy(prev_directory, prev_dir);
}

void hop_implementation()
{
    if (args[1] == NULL) 
    {
        update_prev_dir();
        chdir(home_directory);
    } 
    for(int i=1;args[i]!=NULL;i++)
    {
        if (strcmp(args[i], "-") == 0) {
            if (strlen(prev_directory) == 0) {
                fprintf(stderr, "No previous directory.\n");
            } 
            else 
            {
                printf("%s\n", prev_directory);
                chdir(prev_directory);
            }
        }
        else if (args[i][0] == '~') 
        {
            char path[4096];
            update_prev_dir();
            if (args[i][1] == '\0') {
                chdir(home_directory);
            } 
            else if(args[i][1] == '/')
            {
                // Handle "cd ~/something"
                sprintf(path, "%s%s", home_directory , args[i] + 1);
                chdir(path);
            }
            else // Change directory
            {
                perror("hop error");
            }
            
            working_directory();
        }
        else if (strcmp(args[i], "..") == 0) {
            // Handle "cd .."
            update_prev_dir();
            // if(args[2] != NULL)
            // {
            //     chdir("..");
            //     char cwd[4096];
            //     if (getcwd(cwd, sizeof(cwd)) == NULL) {
            //         perror("getcwd() error");
            //     }
            //     char new_path[4096];
            //     sprintf(new_path, "%s/%s", cwd, args[2]);
            //     if (chdir(new_path) != 0) 
            //     {
            //         perror("hop error");
            //     }
            // }
            // else
            // {
            chdir("..");
            // }
            working_directory();
        }
        else
        {
            update_prev_dir();
            if (chdir(args[i]) != 0) {
                perror("hop error");
            }
            working_directory();
        }
    }
}