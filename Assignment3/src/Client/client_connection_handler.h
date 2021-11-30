#ifndef ASSIGNMENT3_CLIENT_CONNECTION_HANDLER_H
#define ASSIGNMENT3_CLIENT_CONNECTION_HANDLER_H

#include <iostream>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "common.h"

/**
 * Class to manage sending and receiving on the client
 */
class ClientConnectionHandler {
private:
    /**
     * Port number
     */
    int port;

    /**
     * Process id of the client
     */
    pid_t pid;

    /**
     * Machine name of the client
     */
    char *host_name;

    /**
     * Server IP address
     */
    char *server_ip_address;

    /**
     * Request message buffer
     */
    char *request_buffer;

    /**
     * Response message buffer
     */
    char *response_buffer;

    /**
     * File descriptor for connection to the server
     */
    int connection_fd;

    /**
     * Server address
     */
    struct sockaddr_in server_address{};

    /**
     * Creates and connects to socket
     */
    void initialize_socket();

public:
    /**
     * Connection handler constructor
     * @param port Port number
     */
    ClientConnectionHandler(int port, pid_t pid, char *host_name, char *server_ip_address);

    /**
     * Destructor
     */
    ~ClientConnectionHandler();

    /**
     * Send a transaction to the server and get the server response
     * @param n Command to send
     * @return Server transaction id returned from server
     */
    int send_command(int n);
};

#endif //ASSIGNMENT3_CLIENT_CONNECTION_HANDLER_H
