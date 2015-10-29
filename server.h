#ifndef CS428_SERVER_H
#define CS428_SERVER_H

#include <netinet/in.h>

int cs428_server(in_port_t, int input_fd, int output_fd);

#endif // CS428_SERVER_H
