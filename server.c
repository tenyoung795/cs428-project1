#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>

#include "server.h"

#define CS428_TALK_WIP
#ifdef CS428_TALK_WIP
static ssize_t cs428_tee(int input_fd, int output_fd) {
    char buffer[BUFSIZ];
    ssize_t result = read(input_fd, buffer, sizeof(buffer));
    if (result == 0) return 0;
    if (result < 0) {
        return -errno;
    }

    if (write(output_fd, buffer, result) < 0) {
        return -errno;
    }
    return result;
}

static int cs428_talk(int input_fd, int peer_socket, int output_fd) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(input_fd, &fds);
    FD_SET(peer_socket, &fds);

    int nfds = (peer_socket > input_fd ? peer_socket : input_fd) + 1;
    while (true) {
        fd_set ready_fds = fds;
        if (select(nfds, &ready_fds, NULL, NULL, NULL) < 0) {
            return -errno;
        }

        if (FD_ISSET(peer_socket, &ready_fds)) {
            ssize_t result = cs428_tee(peer_socket, output_fd);
            if (result == 0) break;
            if (result < 0) return result;
        }

        if (FD_ISSET(input_fd, &ready_fds)) {
            ssize_t result = cs428_tee(input_fd, peer_socket);
            if (result == 0) break;
            if (result < 0) return result;
        }
    }
    return 0;
}
#endif

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

int cs428_server(in_port_t port, int input_fd, int output_fd) {
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
