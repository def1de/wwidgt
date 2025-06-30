#include <stdio.h>
#include <string.h>

#include "daemon.h"
#include "ipc.h"


int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [daemon|log|variable get/set]\n", argv[0]);
    }

    if (strcmp(argv[1], "daemon") == 0) {
        printf("Running daemon\n");
        run_daemon(argc, argv);
        return 0;
    } else if (strcmp(argv[1], "log") == 0) {
        ipc_send_command("LOG_STREAM");
    } else if (strcmp(argv[1], "variable") == 0 && argc >= 4) {
        char command[256];
        if (strcmp(argv[2], "get") == 0) {
            snprintf(command, sizeof(command), "GET_VAR %s", argv[3]);
            ipc_send_command(command);
        } else if (strcmp(argv[2], "set") == 0 && argc == 5) {
            snprintf(command, sizeof(command), "SET_VAR %s %s", argv[3], argv[4]);
            ipc_send_command(command);
        } else {
            fprintf(stderr, "Invalid variable command\n");
        }
    } else {
        fprintf(stderr, "Unknown command\n");
        return 1;
    }
}