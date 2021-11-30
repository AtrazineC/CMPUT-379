#include <iostream>
#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <climits>
#include <sys/poll.h>

#include "common.h"
#include "server_logger.h"
#include "transaction_handler.h"
#include "server_connection_handler.h"

using namespace std;

/**
 * Server entry point
 */
int main(int argc, char *argv[]) {
    // Check minimum number of arguments was provided
    if (argc < 2) {
        fprintf(stderr, "Error: must include port number.\n");
        exit(EXIT_FAILURE);
    }

    // Get initial data
    int port = atoi(argv[1]);
    pid_t pid = getpid();
    char host_name[HOST_NAME_MAX];

    // Get host name
    if (gethostname(host_name, HOST_NAME_MAX) < 0) {
        fprintf(stderr, "Error: failed to get host name\n");
        exit(EXIT_FAILURE);
    }

    // Verify port number
    if (port < MIN_PORT_VALUE || port > MAX_PORT_VALUE) {
        fprintf(stderr, "Error: port number must be between %d and %d.\n", MIN_PORT_VALUE, MAX_PORT_VALUE);
        exit(EXIT_FAILURE);
    }

    // Initialize logger, transaction handler, and connection handler
    auto *server_logger = new ServerLogger(port, pid, host_name);
    auto *transaction_handler = new TransactionHandler(server_logger);
    auto *connection_handler = new ServerConnectionHandler(port, transaction_handler);

    // Server loops to handle connections
    // Will return false when server times out
    while (connection_handler->run());

    // Close connection and output summary
    server_logger->finish(transaction_handler->get_transaction_count(),
                          transaction_handler->get_client_transaction_map(),
                          transaction_handler->get_first_transaction_start_time(),
                          transaction_handler->get_latest_transaction_finish_time());

    // Clean up
    delete server_logger;
    delete transaction_handler;
    delete connection_handler;

    return EXIT_SUCCESS;
}