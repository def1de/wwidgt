#include "logging.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <sys/socket.h>

#define MAX_LOG_CLIENTS 10
#define MAX_LOG_MESSAGE 1024

static int log_clients[MAX_LOG_CLIENTS];
static int log_client_count = 0;

void add_log_client(int fd) {
    if (log_client_count < MAX_LOG_CLIENTS) {
        log_clients[log_client_count++] = fd;

        // Send previous logs to the new client
        FILE* log_file = fopen("/tmp/wwidgt.log", "r");
        if (log_file) {
            char line[MAX_LOG_MESSAGE];
            while (fgets(line, sizeof(line), log_file)) {
                send(fd, line, strlen(line), 0);
            }
            fclose(log_file);
        } else {
            perror("Failed to open log file");
        }
    }
}

void remove_log_client(int fd) {
    for (int i = 0; i < log_client_count; i++) {
        if (log_clients[i] == fd) {
            for (int j = i; j < log_client_count - 1; j++) {
                log_clients[j] = log_clients[j + 1];
            }
            log_client_count--;
            break;
        }
    }
}

bool is_log_client(int fd) {
    for (int i = 0; i < log_client_count; ++i) {
        if (log_clients[i] == fd) {
            return true;
        }
    }
    return false;
}

void broadcast_log(const char* msg) {
    for (int i = 0; i < log_client_count; ++i) {
        send(log_clients[i], msg, strlen(msg), 0);
    }
}

void log_printf(const char* format, ...) {
    char buffer[MAX_LOG_MESSAGE];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    // Append the log message to a file
    FILE* log_file = fopen("/tmp/wwidgt.log", "a");
    if (log_file) {
        fprintf(log_file, "%s", buffer);
        fclose(log_file);
    } else {
        perror("Failed to open log file");
    }

    // Send to clients
    broadcast_log(buffer);

    // Also write to stdout for debugging (optional)
    printf("%s", buffer);
}
