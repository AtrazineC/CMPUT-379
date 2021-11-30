#include "client_connection_handler.h"

ClientConnectionHandler::ClientConnectionHandler(int port, pid_t pid, char *host_name, char *server_ip_address) {
    this->port = port;
    this->pid = pid;
    this->host_name = host_name;
    this->server_ip_address = server_ip_address;

    // Initialize message buffers
    this->request_buffer = new char[MESSAGE_SIZE];
    this->response_buffer = new char[MESSAGE_SIZE];

    // Initialize socket
    initialize_socket();
}

ClientConnectionHandler::~ClientConnectionHandler() {
    close(connection_fd);
    delete[] request_buffer;
    delete[] response_buffer;
}

int ClientConnectionHandler::send_command(int n) {
    // Write to request message buffer
    snprintf((char *)request_buffer, MESSAGE_SIZE, REQUEST_FORMAT, n, host_name, pid);

    // Send request
    if (send(connection_fd, request_buffer, strlen(request_buffer), 0) < 0) {
        fprintf(stderr, "Error: send failed\n");
        exit(EXIT_FAILURE);
    }

    // Read response
    if (recv(connection_fd, response_buffer, MESSAGE_SIZE, 0) < 0) {
        fprintf(stderr, "Error: receive failed\n");
        exit(EXIT_FAILURE);
    }

    // Process response
    int transaction_id;

    if (sscanf(response_buffer, RESPONSE_FORMAT, &transaction_id) < 0) {
        fprintf(stderr, "Error: parsing request response failed\n");
        exit(EXIT_FAILURE);
    }

    return transaction_id;
}

void ClientConnectionHandler::initialize_socket() {
    // Initialize server address struct
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_ip_address);
    server_address.sin_port = htons(port);

    // Create socket
    if ((connection_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Error: socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    // Connect
    if (connect(connection_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        fprintf(stderr, "Error: connection failed\n");
        exit(EXIT_FAILURE);
    }
}
