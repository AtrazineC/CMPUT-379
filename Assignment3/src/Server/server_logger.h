#ifndef ASSIGNMENT3_SERVER_LOGGER_H
#define ASSIGNMENT3_SERVER_LOGGER_H

#include <string>
#include <cstdio>
#include <cstring>
#include <chrono>
#include <map>

#include "util.h"

#define OUTPUT_FORMAT_START "%.2f: #%3d (T%3d) from %s\n"
#define OUTPUT_FORMAT_FINISH "%.2f: #%3d (Done) from %s\n"
#define OUTPUT_SUMMARY_FORMAT "%4d transactions from %s\n"

using namespace std;
using namespace chrono;

/**
 * Handles logging events to an output file
 */
class ServerLogger {
private:
    /**
     * The output file
     */
    FILE *file;
public:
    /**
     * Constructor
     * @param port Port number
     * @param pid PID of the server
     * @param host_name Name of host machine
     */
    ServerLogger(int port, pid_t pid, char *host_name);

    /**
     * Destructor
     */
    ~ServerLogger();

    /**
     * Log transaction start
     * @param id Server transaction id
     * @param n Transaction argument
     * @param client_machine_name Client machine name
     * @param client_pid Client pid
     */
    void log_start_transaction(int id, int n, char *client_machine_name, pid_t client_pid);

    /**
     * Log transaction finish
     * @param id Server transaction id
     * @param client_machine_name Client machine name
     * @param client_pid Client pid
     */
    void log_finish_transaction(int id, char *client_machine_name, pid_t client_pid);

    /**
     * Output summary
     * @param transaction_count Total transaction count handled by server
     * @param client_transaction_map Map from client name to transactions completed
     * @param start_time Time at start of first transaction
     * @param finish_time Time at end of last transaction
     */
    void finish(int transaction_count, map<string, int> *client_transaction_map,
                double start_time, double finish_time);
};

#endif //ASSIGNMENT3_SERVER_LOGGER_H
