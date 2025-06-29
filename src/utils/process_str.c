#include "process_str.h"

#include <stdlib.h>
#include <string.h>

#include "variables.h"

static char* run_shell_command(const char* cmd) {
    FILE* fp = popen(cmd, "r");
    if (!fp) return strdup("[error]");

    char buffer[256];
    size_t total = 0;
    char* result = NULL;

    while (fgets(buffer, sizeof(buffer), fp)) {
        size_t len = strlen(buffer);
        result = realloc(result, total + len + 1);
        memcpy(result + total, buffer, len);
        total += len;
        result[total] = '\0';
    }

    pclose(fp);
    return result;
}

char* process_str(const char* input) {
    size_t len = strlen(input);
    char* output = malloc(1);
    output[0] = '\0';
    size_t out_len = 0;

    for (size_t i = 0; i < len; ) {
        if (strncmp(&input[i], "{{", 2) == 0) {
            size_t end = strstr(&input[i], "}}") - input;
            char var_name[128] = {0};
            strncpy(var_name, &input[i + 2], end - i - 2);
            const char* value = get_variable_value(var_name);

            size_t val_len = strlen(value);
            output = realloc(output, out_len + val_len + 1);
            strcpy(output + out_len, value);
            out_len += val_len;

            i = end + 2;
        }
        else if (strncmp(&input[i], "{%", 2) == 0) {
            size_t end = strstr(&input[i], "%}") - input;
            char cmd[256] = {0};
            strncpy(cmd, &input[i + 2], end - i - 2);
            while (cmd[0] == ' ') memmove(cmd, cmd + 1, strlen(cmd)); // trim leading spaces

            char* result = run_shell_command(cmd);
            if (result) {
                size_t val_len = strlen(result);
                output = realloc(output, out_len + val_len + 1);
                strcpy(output + out_len, result);
                out_len += val_len;
                free(result);
            }

            i = end + 2;
        }
        else {
            output = realloc(output, out_len + 2);
            output[out_len++] = input[i++];
            output[out_len] = '\0';
        }
    }

    return output;
}