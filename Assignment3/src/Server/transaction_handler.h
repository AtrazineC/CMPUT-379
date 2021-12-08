#ifndef ASSIGNMENT3_TRANSACTION_HANDLER_H
#define ASSIGNMENT3_TRANSACTION_HANDLER_H

#include <map>

#include "tands.h"
#include "util.h"
#include "server_logger.h"

/**
 * Class for performing transactions and keeping track of
 * number of transactions completed
 */
class TransactionHandler {
private:
    /**
     * Logger instance
     */
    ServerLogger *server_logger;

    /**
     * Current transaction count
     */
    int transaction_count = 0;

    /**
     * Time at start of first transaction
     */
    double first_transaction_start_time;

    /**
     * Time at finish of latest transaction
     */
    double latest_transaction_finish_time;

    /**
     * Map from client name to the number of transactions fulfilled by the client
     */
    map<string, int> client_transaction_map;
public:
    /**
     * Constructor
     * @param server_logger The logger
     */
    TransactionHandler(ServerLogger *server_logger);

    /**
     * Get total transaction count
     * @return Total transaction count
     */
    int get_transaction_count();

    /**
     * Get time at start of first transaction
     * @return Time at start of first transaction
     */
    double get_first_transaction_start_time();

    /**
     * Get time at end of last transaction
     * @return Time at end of last transaction
     */
    double get_latest_transaction_finish_time();

    /**
     * Client transaction map
     * @return Pointer to client transaction map
     */
    map<string, int> *get_client_transaction_map();

    /**
     * Performs a transaction and returns transaction id
     * @param n Transaction argument
     * @param client_machine_name Client machine name
     * @param client_pid Client pid
     * @return
     */
    int perform_transaction(int n, char *client_machine_name, pid_t client_pid);
};

#endif //ASSIGNMENT3_TRANSACTION_HANDLER_H
