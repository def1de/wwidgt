#include "ipc.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/variables.h"
#include "utils/logging.h"

#define SOCKET_PATH "/tmp/wwidgt.sock"

void ipc_send_command(const char* command) {
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr = {0};

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        exit(1);
    }

    send(sock, command, strlen(command), 0);

    char buffer[1024];
    while (1) {
        int bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) break;
        buffer[bytes] = '\0';
        fputs(buffer, stdout); // print log
        fflush(stdout);
    }
    close(sock);
}


void* ipc_start_server(void* arg) {
    printf("Starting IPC server...\n");
    unlink(SOCKET_PATH);
    int server = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr = {0};

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    bind(server, (struct sockaddr*)&addr, sizeof(addr));
    listen(server, 5);

    while (1) {
        int client = accept(server, NULL, NULL);
        char buffer[1024] = {0};
        int bytes = recv(client, buffer, sizeof(buffer) - 1, 0);

        if (bytes > 0) {
            buffer[bytes] = '\0';

            // Process commands
            if (strncmp(buffer, "GET_VAR ", 8) == 0) {
                const char* key = buffer + 8;
                const char* val = get_variable_value(key);
                send(client, val ? val : "Not found", strlen(val ? val : "Not found"), 0);
            } else if (strncmp(buffer, "SET_VAR ", 8) == 0) {
                char* space = strchr(buffer + 8, ' ');
                if (space) {
                    *space = '\0';
                    const char* key = buffer + 8;
                    const char* value = space + 1;
                    if (set_variable_value(key, value)) {
                        send(client, "OK", 2, 0);
                    } else {
                        send(client, "ERR", 3, 0);
                    }
                }
            } else if (strncmp(buffer, "LOG_STREAM", 10) == 0) {
                add_log_client(client);
                log_printf("Client %d connected for log streaming\n", client);
                continue;
            }
        }

        if (!is_log_client(client)) {
            close(client);
        }
    }

    close(server);
    unlink(SOCKET_PATH);
}
