#include "fd_array_handler.h"

FDArrayHandler::FDArrayHandler(int max_size, int listen_fd) {
    // Initialize file descriptor array
    fd_array = new struct pollfd[max_size];

    // Set up stuff for poll
    memset(fd_array, 0, max_size);

    // Add listen file descriptor (this is the file
    // descriptor for listening for new connections)
    add_connection(listen_fd);
}

FDArrayHandler::~FDArrayHandler() {
    delete[] fd_array;
}

struct pollfd *FDArrayHandler::get_array() {
    return fd_array;
}

int FDArrayHandler::get_count() {
    return fd_count;
}

void FDArrayHandler::add_connection(int new_fd) {
    // Add the file descriptor to the array and
    // increment the array size
    fd_array[fd_count].fd = new_fd;
    fd_array[fd_count].events = POLLIN;
    fd_count++;
}

void FDArrayHandler::remove_connection(int fd) {
    // Find this file descriptor in the array and mark
    // it for removal when compress_array is called
    for (int i = 0; i < fd_count; i++) {
        if (fd_array[i].fd == fd) {
            fd_array[i].fd = -1;
            break;
        }
    }
}

void FDArrayHandler::compress_array() {
    for (int i = 0; i < fd_count; i++) {
        // If this item is marked for cleanup, shift items
        // ahead of it into its spot
        if (fd_array[i].fd == -1) {
            for (int j = i; j < fd_count; j++) {
                fd_array[j].fd = fd_array[j + 1].fd;
            }

            fd_count--;
            i--;
        }
    }
}
