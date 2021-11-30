#include <iostream>
#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <climits>

#include "common.h"
#include "command.h"
#include "client_logger.h"
#include "tands.h"
#include "client_connection_handler.h"

#define IP_MAX_LENGTH 100

using namespace std;

/**
 * Client entry point
 */
int main(int argc, char *argv[]) {
    // Check minimum number of arguments was provided
    if (argc < 3) {
        fprintf(stderr, "Error: must include port number and ip address.\n");
        exit(EXIT_FAILURE);
    }

    // Get initial data
    int port = atoi(argv[1]);
    pid_t pid = getpid();
    char host_name[HOST_NAME_MAX];
    char server_ip_address[IP_MAX_LENGTH];

    // Get host name
    if (gethostname(host_name, HOST_NAME_MAX) < 0) {
        fprintf(stderr, "Error: failed to get host name\n");
        exit(EXIT_FAILURE);
    }

    // Get ip address
    sprintf(server_ip_address, "%s", argv[2]);

    // Verify port number
    if (port < MIN_PORT_VALUE || port > MAX_PORT_VALUE) {
        fprintf(stderr, "Error: port number must be between %d and %d.\n", MIN_PORT_VALUE, MAX_PORT_VALUE);
        exit(EXIT_FAILURE);
    }

    // Initialize logger and connection handler
    auto *client_logger = new ClientLogger(port, pid, host_name, server_ip_address);
    auto *client_connection_handler = new ClientConnectionHandler(port, pid, host_name, server_ip_address);

    // Consume commands from standard input
    Command command = get_next_command();

    while (command.command_type != Command::CommandType::close) {
        if (command.valid) {
            // Log command
            client_logger->log_command(&command);

            // Process command
            if (command.command_type == Command::CommandType::transaction) {
                // Send command argument to server
                int transaction_id = client_connection_handler->send_command(command.n);

                // Log response
                client_logger->log_receive(transaction_id);
            } else if (command.command_type == Command::CommandType::sleep) {
                // Sleep
                Sleep(command.n);
            }
        }

        command = get_next_command();
    }

    // Close connection and output summary
    client_logger->finish();

    // Clean up
    delete client_logger;
    delete client_connection_handler;

    return EXIT_SUCCESS;
}