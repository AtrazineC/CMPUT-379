#include "transaction_handler.h"

TransactionHandler::TransactionHandler(ServerLogger *server_logger) {
    this->server_logger = server_logger;
    this->first_transaction_start_time = get_epoch_time();
    this->latest_transaction_finish_time = get_epoch_time();
    this->client_transaction_map = map<string, int>();
}

int TransactionHandler::perform_transaction(int n, char *client_machine_name, pid_t client_pid) {
    int transaction_id = ++transaction_count;

    // Log transaction start
    server_logger->log_start_transaction(transaction_id, n, client_machine_name, client_pid);

    // Perform transaction
    double start_time = get_epoch_time();
    Trans(n);
    double finish_time = get_epoch_time();

    // Save start time if first transaction
    if (transaction_id == 1) {
        first_transaction_start_time = start_time;
    }

    // Save latest transaction finish time
    latest_transaction_finish_time = finish_time;

    // Log transaction end
    server_logger->log_finish_transaction(transaction_id, client_machine_name, client_pid);

    // Update client transaction map
    string client_name = get_client_name(client_machine_name, client_pid);

    if (client_transaction_map.contains(client_name)) {
        client_transaction_map.at(client_name)++;
    } else {
        client_transaction_map.emplace(client_name, 1);
    }

    return transaction_id;
}

int TransactionHandler::get_transaction_count() {
    return transaction_count;
}

double TransactionHandler::get_first_transaction_start_time() {
    return first_transaction_start_time;
}

double TransactionHandler::get_latest_transaction_finish_time() {
    return latest_transaction_finish_time;
}

map<string, int> *TransactionHandler::get_client_transaction_map() {
    return &client_transaction_map;
}
