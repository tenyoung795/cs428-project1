#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>

#include "talk.h"

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

int cs428_talk(int input_fd, int peer_socket, int output_fd) {
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
