#include"input.h"
#include"globals.h"

char* join_strings_with_space(const char *str1, const char *str2) {
    // Allocate memory for the resulting string
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char *result = (char *)malloc(len1 + len2 + 2); // +1 for the space and +1 for the null terminator

    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // Copy the first string
    strcpy(result, str1);

    strcat(result, " ");
    
    // Concatenate the second string
    strcat(result, str2);

    return result;
}