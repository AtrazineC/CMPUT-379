#ifndef ASSIGNMENT3_SERVER_CONNECTION_HANDLER_H
#define ASSIGNMENT3_SERVER_CONNECTION_HANDLER_H

#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <climits>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/poll.h>

#include "common.h"
#include "server_logger.h"
#include "transaction_handler.h"
#include "fd_array_handler.h"

#define MAX_CLIENTS 50
#define MAX_IDLE_TIME_MS 30000
#define MAX_BACKLOG_SIZE 10

/**
 * Class to manage incoming connections to the server
 */
class ServerConnectionHandler {
private:
    /**
     * Value for socket options
     */
    const int opt_val = 1;

    /**
     * Port number
     */
    int port;

    /**
     * Request message buffer
     */
    char *request_buffer;

    /**
     * Response message buffer
     */
    char *response_buffer;

    /**
     * File descriptor for listening to new connections
     */
    int listen_fd;

    /**
     * Server address
     */
    struct sockaddr_in server_address{};

    /**
     * File descriptor array handler. Provides an abstraction
     * for the file descriptor array passed into poll
     */
    FDArrayHandler *fd_array_handler;

    /**
     * Transaction handler. Provides an abstraction for
     * running transactions and keeping track of the number
     * of transactions completed
     */
    TransactionHandler *transaction_handler;

    /**
     * Creates, binds, and listens to socket
     */
    void initialize_socket();

    /**
     * Accept a connection
     */
    void accept_connection();

    /**
     * Receive data at the given file descriptor
     * @param client_fd File descriptor to receive data from
     */
    void receive_data(int client_fd);

public:
    /**
     * Connection handler constructor
     * @param port Port number
     * @param transaction_handler Transaction handler
     */
    ServerConnectionHandler(int port, TransactionHandler *transaction_handler);

    /**
     * Destructor
     */
    ~ServerConnectionHandler();

    /**
     * Run the connection handler
     * @return Returns false when finished running, true otherwise
     */
    bool run();
};

#endif //ASSIGNMENT3_SERVER_CONNECTION_HANDLER_H
