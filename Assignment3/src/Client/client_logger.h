#ifndef ASSIGNMENT3_CLIENT_LOGGER_H
#define ASSIGNMENT3_CLIENT_LOGGER_H

#include <cstdio>
#include <cstring>
#include <chrono>

#include "command.h"
#include "util.h"

#define OUTPUT_FORMAT_TRANSACTION "%.2f: %4s (%1s%3d)\n"
#define OUTPUT_FORMAT_SLEEP "Sleep %d units\n"
#define OUTPUT_FORMAT_RECEIVE "%.2f: %4s (%1s%3d)\n"

using namespace std;
using namespace chrono;

/**
 * Handles logging events to an output file
 */
class ClientLogger {
private:
    /**
     * The output file
     */
    FILE *file;

    /**
     * Transaction count of this logger
     */
    int transaction_count = 0;

    /**
     * Log transaction
     * @param n Transaction argument
     */
    void log_transaction(int n);

    /**
     * Log sleep
     * @param n Sleep argument
     */
    void log_sleep(int n);

public:
    /**
     * Constructor
     * @param port Port number
     * @param pid PID of the client
     * @param host_name Name of host machine
     * @param server_ip_address Server IP address
     */
    ClientLogger(int port, pid_t pid, char *host_name, char *server_ip_address);

    /**
     * Destructor
     */
    ~ClientLogger();

    /**
     * Log a command to output file
     * @param command The command processed
     */
    void log_command(Command *command);

    /**
     * Log a receive event to output file
     * @param result_id The id received from the server
     */
    void log_receive(int result_id);

    /**
     * Output summary
     */
    void finish();
};

#endif //ASSIGNMENT3_CLIENT_LOGGER_H
