#include"log.h"
#include"globals.h"
#include <fcntl.h>  // For open
#include <unistd.h> // For write and close
#include <string.h>
// char command_log[LOG_SIZE][1024];
int latest_log = -1;

void save_log();

void update_latest_log() {
    snprintf(command_log[0], sizeof(command_log[0]), "%d", latest_log);
}
void load_log() {
    FILE *file = fopen(file_directory, "r");
    if (file) {
        // Read the latest_log value from the first line
        fscanf(file, "%d\n", &latest_log);
        latest_index = latest_log;
        // Read the command_log entries
        int log_index = 0;
        log_count = 0;
        while (fgets(command_log[log_index], sizeof(command_log[log_index]), file)) 
        {
            command_log[log_index][strcspn(command_log[log_index], "\n")] = 0;

            if (command_log[log_index][0] != '\0' && strspn(command_log[log_index], " \t") != strlen(command_log[log_index])) 
            {
                // Process the non-empty line
                log_index++;
                log_count++;
            }
        }

        fclose(file);
    }  
    else 
    {
        printf("Failed to open file.\n"); // Debug print
    }
}

void save_log() {
    int fd = open(file_directory, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    if (fd != -1) {
        char buffer[32];
        int len = snprintf(buffer, sizeof(buffer), "%d\n", latest_log);
        write(fd, buffer, len);
        
        // Write the command_log entries
        for (int i = 0; i < log_count; i++) {
            write(fd, command_log[i], strlen(command_log[i]));
            write(fd, "\n", 1);
        }
        close(fd);
    } else {
        perror("Failed to open file for writing");
    }
}

void add_to_log(const char *command) {
    if (latest_log >= 0 && strcmp(command_log[latest_log], command) == 0) {
        return; // Do not add duplicate consecutive commands
    }
    latest_log = (latest_log + 1) % LOG_SIZE;
    strcpy(command_log[latest_log], command);
    if (log_count < LOG_SIZE) {
        log_count++;
    }
    latest_index = latest_log;
    save_log();
}

void clear_file() {
    FILE *file = fopen(file_directory, "w");
    latest_log = -1;
    latest_index = -1;
    for (int i = 0; i < LOG_SIZE; i++) 
    {
        memset(command_log[i], 0, sizeof(command_log[i]));  // Clear each entry in the command_log array
    }
    fprintf(file, "%d\n", latest_log);
    log_count = 0;
    if (file) {
        fclose(file);
        printf("File cleared successfully: %s\n", file_directory);
    } else {
        perror("Failed to open file");
    }
}

void display_log() {
    // for (int i = 0; i < log_count; i++) {
    //     if (command_log[i] == NULL) {
    //         printf("Null entry at index %d\n", i); // Debug print
    //         continue;
    //     }
    //     printf("%d: %s\n", i + 1, command_log[i]);
    // }
    int count = 1;
    for(int i=latest_log ; i>=0; i--)
    {
        if(command_log[i] == NULL)
        {
            continue;
        }
        printf("%d: %s\n", count++ , command_log[i]);
    }
    for(int i=LOG_SIZE-1 ; i>latest_log; i--)
    {
        if(command_log[i] == NULL)
        {
            continue;
        }
        printf("%d: %s\n",count++ , command_log[i]);
    }
}