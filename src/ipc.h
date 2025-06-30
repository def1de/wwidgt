#ifndef IPC_H
#define IPC_H

void ipc_send_command(const char* command);
void* ipc_start_server(void* arg);

#endif //IPC_H
