#include"hop.h"
#include"reveal.h"
#include"print.h"
#include"globals.h"
#include"pwd.h"
#include <ctype.h>
#include"log.h"
#include"proclore.h"
#include"seek.h"    
#include"input.h"
#include <fcntl.h>  // For open
#include <unistd.h> // For write and close
#include <string.h>

int flag = 1;
int exec_flag = 0;
void replace_tabs_with_spaces(char *str) 
{
    for (int i = 0; str[i] != '\0'; i++) 
    {
        if (str[i] == '\t') {
            str[i] = ' ';
        }
    }
}

int execute_command(char *command);
void run_processes()
{
    
}
void handle_input(char *input) {
    replace_tabs_with_spaces(input);
    int command_count = 0;
    char *commands[1024];
    
    
    char *command = strtok(input, ";");
    for (command_count = 0; command != NULL; command_count++) 
    {
        commands[command_count] = command;
        command = strtok(NULL, ";");
    }
    commands[command_count] = NULL;
    for (int i = 0; i < command_count; i++) 
    {   
        char f_or_b = 'b';
        int b_count = 0;
        char *background_commands[1024];
        if(strchr(commands[i],'&') == NULL)
        {
            time_t start_time, end_time;
            time(&start_time);
            execute_command(commands[i]);
            time(&end_time);
            double time_taken = difftime(end_time, start_time);
            // int rounded_time = (int)time_taken;
            // if(rounded_time > 2)
            // {
            //     strcpy(time_stamp , commands[i]);
            //     printf("%s",time_stamp);
            // }
        }
        else
        {
            char *sub_commands[1024];
            int sub_command_count = 0;
            int l = strlen(commands[i]);
            if(commands[i][l-1] == '&')
            {
                // now tokenise it according to &
                char *sub_command = strtok(commands[i], "&");
                for (sub_command_count = 0; sub_command != NULL; sub_command_count++) 
                {
                    sub_commands[sub_command_count] = sub_command;
                    sub_command = strtok(NULL, "&");
                }
                sub_commands[sub_command_count] = NULL;
                for(int j = 0; j < sub_command_count; j++)
                {
                    background_commands[b_count] = sub_commands[j];
                    b_count++;
                }
            }
            else
            {
                char *sub_command = strtok(commands[i], "&");
                for (sub_command_count = 0; sub_command != NULL; sub_command_count++) 
                {
                    sub_commands[sub_command_count] = sub_command;
                    sub_command = strtok(NULL, "&");
                }
                sub_commands[sub_command_count] = NULL;
                for(int j = 0; j < sub_command_count; j++)
                {
                    background_commands[b_count] = sub_commands[j];
                    b_count++;
                }
                f_or_b = 'f';
            }
        }
        for(int i = 0; i < b_count; i++)
        {
            if(f_or_b == 'f' && i == b_count - 1)
            {
                time_t start_time, end_time;
                time(&start_time);
                execute_command(background_commands[i]);
                time(&end_time);
                double time_taken = difftime(end_time, start_time);
                int rounded_time = (int)time_taken;
                // if(rounded_time > 2)
                // {
                //     strcpy(time_stamp , background_commands[i]);
                //     printf("%s",time_stamp);
                // }
                continue;
            }
            trim_whitespace(background_commands[i]);
            pid_t child_id = 0;
            pid_t pid = fork();
            if(pid == 0)
            {
                child_id = getpid();
                printf("%d\n", child_id);
                // File* file = fopen(background_directory , 'a');

                int s = execute_command(background_commands[i]);
                
                FILE *file = fopen(background_directory, "a");
                if (file != NULL) {
                    // Write the formatted string to the file
                    if(exec_flag != 1)
                    {
                        fprintf(file, "%s exited normally (%d)\n", background_commands[i], child_id);
                    }
                    // else
                    // {
                    //     fprintf(file, "%s exited abnormally (%d)\n", background_commands[i], child_id);
                    // }
                    // Close the file
                    fclose(file);
                } else {
                    perror("fopen");
                }
                exit(EXIT_SUCCESS);
            }
            else if(pid < 0)
            {
                perror("fork");
            }
            else
            {
                // int fd = open(background_directory, O_WRONLY | O_APPEND | O_CREAT, 0644);
                // if(fd != -1)
                // {
                //     char buffer[32];
                //     snprintf(buffer, sizeof(buffer), "%d\n", pid);
                //     write(fd, buffer, strlen(buffer));
                //     close(fd);
                // }
                // else
                // {
                //     perror("Failed to open file for writing");
                // }
            }
        }     
    }
}

int execute_command(char *command) 
{
    replace_tabs_with_spaces(command);
    char str[1024];
    strcpy(str, command);
    for (int j = 0; j < MAX_ARGS; j++) 
    {
        args[j] = NULL;
    }

    char *token = strtok(str, " ");
    int i = 0;
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    if (strcmp(args[0], "hop") == 0) {
        hop_implementation();
    } 
    else if (strcmp(args[0], "reveal") == 0) {
        reveal_implementation();
    } 
    else if (strcmp(args[0], "pwd") == 0) {
        working_directory();
    } 
    else if(strcmp(args[0] , "seek") == 0)
    {
        file_to_print = (char*)malloc(sizeof(char)*4096);
        dir_to_change = (char*)malloc(sizeof(char)*4096);
        int flag_d = 0;
        int flag_f = 0;
        int flag_e = 0;
        file_count = 0;
        dir_count = 0;
        
        // printf("Seeking\n");
        if(args[1][0] != '-')
        {
            char cwd[4096];
            if(args[2]!=NULL)
                seek(args[1] , args[2] , 0 , 0 , 0);
            else
            {
                if (getcwd(cwd, sizeof(cwd)) == NULL) 
                {
                    perror("getcwd() error");
                }
                seek(args[1] , cwd , 0 , 0 , 0);
            }
        }
        else
        {
            for(int i=1;args[i]!=NULL;i++)
            {
                if(args[i][0] == '-')
                {
                    if(args[i][1] == 'd')
                    {
                        flag_d = 1;
                    }
                    if(args[i][1] == 'f')
                    {
                        flag_f = 1;
                    }
                    if(args[i][1] == 'e')
                    {
                        flag_e = 1;
                    }
                }
                else
                {
                    if(args[i+1]!=NULL)
                    {
                        seek(args[i] , args[i+1] , flag_d , flag_f , flag_e);
                    }
                    else
                    {
                        char cwd[4096];
                        if (getcwd(cwd, sizeof(cwd)) == NULL) 
                        {
                            perror("getcwd() error");
                        }
                        seek(args[i] , cwd , flag_d , flag_f , flag_e);
                    }
                    break;
                }
            }
        }
        if(flag_d && flag_f)
        {
            printf("Invalid flags\n");
        }
        else if(file_count == 0 && dir_count == 0)
        {
            printf("No Match found\n");
        }
        else if (flag_e) {
            if (file_count == 1 && dir_count == 0) {
                if (access(file_to_print, R_OK) == 0) {
                    FILE *file = fopen(file_to_print, "r");
                    if (file) {
                        char buffer[1024];
                        while (fgets(buffer, sizeof(buffer), file) != NULL) {
                            printf("%s", buffer);
                        }
                        printf("\n");
                        fclose(file);
                    } 
                    else 
                    {
                        perror("fopen");
                    }
                } 
                else {
                    printf("Missing permissions for task!\n");
                }
            } 
            else if (dir_count == 1 && file_count == 0) {
                if (access(dir_to_change, X_OK) == 0) {
                    if (chdir(dir_to_change) == 0) {
                        // printf("Changed directory to %s\n", dir_to_change);
                    } else {
                        perror("chdir");
                    }
                } else {
                    printf("Missing permissions for task!\n");
                }
            }
        }
    }
    else if(strcmp(args[0] , "proclore") == 0)
    {
        if(args[1] != NULL)
        {
            int pid = atoi(args[1]);
            proclore(pid);
        }
        else 
        {
            proclore(getpid());
        }
    }
    else if(strcmp(args[0] , "log") == 0)
    {
        if((args[1]!=NULL))
        {
            if(strcmp(args[1],"purge") == 0)
            {
                clear_file();
            }
            if(strcmp(args[1],"execute") == 0)
            {
                int index = atoi(args[2]);
                if(index < LOG_SIZE && index <= log_count)
                {
                    int index1 = latest_index - index + 1;
                    // printf("%d\n",latest_index);
                    if(index1<0)
                    {
                        index1 += 15;
                    }
                    
                    if(command_log[index1]!="")
                    {
                        // printf("%s\n",command_log[index1]);
                        execute_command(command_log[index1]);
                        if(index != 1)
                        {
                            add_to_log(command_log[index1]);
                        }
                    }
                }
                else
                {
                    printf("Command does not exist at this index\n");
                }
            }
        }
        else
        {
            flag = 0;
            display_log();
        }
    }
    else if(strcmp(args[0] , "exit") == 0)
    {
        exit(0);
    }
    else {
        // printf("Executing command: %s\n", command);
        exec_flag = 0;
        // if(system(command) != 0)
        // {
        //     return -1;
        // }
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            execvp(args[0], args);
            // If execvp returns, there was an error
            perror("execvp failed");
            exec_flag = 1;
            return -1;
            exit(EXIT_FAILURE);
        } 
        else if (pid > 0) 
        {
            // Parent process
            int status;
            waitpid(pid, &status, 0);
        } 
        else {
            // Fork failed
            perror("fork failed");
            return -1;
        }
    }
}

int main()
{
    char cwd[4096];
    if (getcwd(cwd, sizeof(cwd)) == NULL) 
    {
        perror("getcwd() error");
    }
    home_directory = malloc(strlen(cwd) + 1);
    strcpy(home_directory, cwd);
    // time_stamp = (char*)malloc(sizeof(char)*4096);
    // strcpy(time_stamp , " ");
    print_direc();
    while(1)
    {
        char input[1024];
        char logg[1024];
        flag = 1;
        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("fgets() error");
            return 1;
        }
        int h = 0;
        input[strcspn(input, "\n")] = 0;
        file_directory = malloc(sizeof(char)*4096);
        background_directory = malloc(sizeof(char)*4096);
        sprintf(file_directory , "%s/src_code/command_log.txt" , home_directory);
        sprintf(background_directory , "%s/src_code/process.txt" , home_directory);
        load_log();

        FILE *file = fopen(background_directory, "r");
        if (file != NULL) {
            char line[1024];
            while (fgets(line, sizeof(line), file) != NULL) {
                printf("%s", line);
            }
            fclose(file);
        } else {
            perror("fopen");
        }

        file = fopen(background_directory, "w");
        if (file != NULL) {
            fclose(file);
        } else {
            perror("fopen");
        }
        strcpy(logg , input);
        handle_input(input);

        if(flag == 1 && strncmp(logg , "log" , 3) != 0)
        {
            // printf("%s\n",logg);
            add_to_log(logg);
        }
        print_direc();
    }
}