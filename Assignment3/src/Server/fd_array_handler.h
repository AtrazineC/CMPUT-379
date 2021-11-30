#ifndef ASSIGNMENT3_FD_ARRAY_HANDLER_H
#define ASSIGNMENT3_FD_ARRAY_HANDLER_H

#include <cstring>
#include <sys/ioctl.h>
#include <sys/poll.h>

#include "common.h"

/**
 * Provides an abstraction for handling the
 * file descriptor array used for poll
 */
class FDArrayHandler {
private:
    /**
     * Array that stores active file descriptors
     */
    struct pollfd *fd_array;

    /**
     * Current file descriptor count
     */
    int fd_count = 0;
public:
    /**
     * Constructor
     * @param max_size Maximum number of file descriptors
     * @param listen_fd File descriptor for listening to new connections
     */
    FDArrayHandler(int max_size, int listen_fd);

    /**
     * Destructor
     */
    ~FDArrayHandler();

    /**
     * Get a pointer to the file descriptor array
     * @return A pointer to the file descriptor array
     */
    struct pollfd *get_array();

    /**
     * Get the number of file descriptors
     * @return Number of file descriptors
     */
    int get_count();

    /**
     * Add a new connection to the file descriptor array
     * @param new_fd File descriptor of new connection to add
     */
    void add_connection(int new_fd);

    /**
     * Remove a connection from the file descriptor array
     * @param fd File descriptor of the connection to remove
     */
    void remove_connection(int fd);

    /**
     * Clean up the file descriptor array by shifting
     * active file descriptors into the spots of empty
     * file descriptors
     */
    void compress_array();
};

#endif //ASSIGNMENT3_FD_ARRAY_HANDLER_H
