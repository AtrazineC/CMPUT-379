#include "server_connection_handler.h"

ServerConnectionHandler::ServerConnectionHandler(int port, TransactionHandler *transaction_handler) {
    this->port = port;
    this->transaction_handler = transaction_handler;

    // Initialize message buffers
    this->request_buffer = new char[MESSAGE_SIZE];
    this->response_buffer = new char[MESSAGE_SIZE];

    // Initialize socket
    initialize_socket();

    // Initialize file descriptor handler
    this->fd_array_handler = new FDArrayHandler(MAX_CLIENTS, listen_fd);
}

ServerConnectionHandler::~ServerConnectionHandler() {
    close(listen_fd);
    delete[] request_buffer;
    delete[] response_buffer;
    delete fd_array_handler;
}

bool ServerConnectionHandler::run() {
    // Grab file descriptor array and count
    struct pollfd *fd_array = fd_array_handler->get_array();
    int fd_count = fd_array_handler->get_count();

    // Poll to handle connections
    // Times out after MAX_IDLE_TIME_MS of no transactions processed/new connections
    int poll_result = poll(fd_array, fd_count, MAX_IDLE_TIME_MS);

    if (poll_result == 0) {
        // Return false to signify the server is done
        fprintf(stdout, "Server timed out\n");
        return false;
    } else if (poll_result < 0) {
        fprintf(stderr, "Error: poll failed\n");
        exit(EXIT_FAILURE);
    }

    // Find readable file descriptors
    for (int i = 0; i < fd_count; i++) {
        pollfd poll_fd = fd_array[i];

        // Check if number of events greater than 0
        if (poll_fd.revents > 0) {
            if (poll_fd.fd == listen_fd) {
                // Listening connection -> Accept incoming connection
                accept_connection();
            } else {
                // Active client connection -> Receive data
                receive_data(poll_fd.fd);
            }
        }
    }

    // Let file descriptor handler it can clean up
    fd_array_handler->compress_array();

    return true;
}

void ServerConnectionHandler::initialize_socket() {
    // Initialize server address struct
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);

    // Create socket
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Error: socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    // Allow socket to be reusable
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt_val, sizeof(opt_val)) < 0) {
        fprintf(stderr, "Error: setting socket option failed\n");
        exit(EXIT_FAILURE);
    }

    // Set socket to be non-blocking
    if (ioctl(listen_fd, FIONBIO, (char *)&opt_val) < 0) {
        fprintf(stderr, "Error: setting socket to be non-blocking failed\n");
        exit(EXIT_FAILURE);
    }

    // Bind
    if (bind(listen_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        fprintf(stderr, "Error: bind failed\n");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(listen_fd, MAX_BACKLOG_SIZE) < 0) {
        fprintf(stderr, "Error: listen failed\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "Awaiting connections\n");
}

void ServerConnectionHandler::accept_connection() {
    int client_fd;

    // Accept the connection
    if ((client_fd = accept(listen_fd, nullptr, nullptr)) < 0) {
        fprintf(stderr, "Error: accept failed\n");
        exit(EXIT_FAILURE);
    }

    // Add new connection to array of file descriptors
    fd_array_handler->add_connection(client_fd);
    fprintf(stdout, "Client connected\n");
}

void ServerConnectionHandler::receive_data(int client_fd) {
    // Receive the data into the request buffer
    ssize_t request_message_size = recv(client_fd, request_buffer, MESSAGE_SIZE, 0);

    if (request_message_size > 0) {
        int n;
        char client_machine_name[HOST_NAME_MAX];
        pid_t client_pid;

        if (sscanf(request_buffer, REQUEST_FORMAT, &n, client_machine_name, &client_pid) < 0) {
            fprintf(stderr, "Error: parsing request message failed\n");
            exit(EXIT_FAILURE);
        }

        // Perform transaction
        int transaction_id = transaction_handler->perform_transaction(n, client_machine_name, client_pid);

        // Create response message (simply the server transaction id)
        snprintf((char *)response_buffer, MESSAGE_SIZE, "%d", transaction_id);

        // Send response_buffer
        if (send(client_fd, response_buffer, strlen(response_buffer), 0) < 0) {
            fprintf(stderr, "Error: send failed\n");
            exit(EXIT_FAILURE);
        }
    } else if (request_message_size == 0) {
        // Close the file descriptor and remove the connection from the fd array
        close(client_fd);
        fd_array_handler->remove_connection(client_fd);
        fprintf(stdout, "Client disconnected\n");
    } else if (request_message_size < 0) {
        fprintf(stderr, "Error: read failed\n");
        exit(EXIT_FAILURE);
    }
}
