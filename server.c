#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "talk.h"

static int cs428_listener(in_port_t port) {
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0) {
        return -errno;
    }

    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr = { htonl(INADDR_ANY) },
    };
    if (bind(listener, (struct sockaddr *)&address, sizeof(address))
        || listen(listener, 1)) {
        int result = -errno;
        close(listener);
        return result;
    }
    return listener;
}

static int cs428_server(in_port_t port, int input_fd, int output_fd) {
    int listener = cs428_listener(port);
    if (listener < 0) {
        return listener;
    }
    
    int client = accept(listener, NULL, NULL);
    int result = -errno;
    close(listener);
    if (client < 0) {
        return result;
    }

    result = cs428_talk(input_fd, client, output_fd);
    close(client);
    return result;
}

int main(int argc, const char *const argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s PORT\n", argv[0]);
        return EXIT_FAILURE;
    }

    int result = cs428_server(atoi(argv[1]), STDIN_FILENO, STDOUT_FILENO);
    if (result < 0) {
        fprintf(stderr, "%s: %s\n", argv[0], strerror(-result));
        return EXIT_FAILURE;
    }
}
