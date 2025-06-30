#ifndef LOGGING_H
#define LOGGING_H

void add_log_client(int fd);
void remove_log_client(int fd);
void broadcast_log(const char* msg);
void log_printf(const char* format, ...);
bool is_log_client(int fd);

#endif //LOGGING_H
