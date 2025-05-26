#include "string_manip.h"
char* concat_str(const char* prefix, const char* title) {
    // Calculate the total length of the resulting string
    size_t total_length = strlen(prefix) + strlen(title) + 1; // +1 for the null terminator

    // Allocate memory for the resulting string
    char* result = malloc(total_length);
    if (!result) {
        perror("Failed to allocate memory");
        return NULL;
    }

    // Copy the prefix and concatenate the title
    strcpy(result, prefix);
    strcat(result, title);

    return result; // Caller must free the memory
}