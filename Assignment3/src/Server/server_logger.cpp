#include "server_logger.h"

ServerLogger::ServerLogger(int port, pid_t pid, char *host_name) {
    // Create/open output file
    string output_name = get_client_name(host_name, pid);
    file = fopen(output_name.c_str(), "w");

    // Initial logging
    fprintf(file, "Using port %d\n\n", port);
}

ServerLogger::~ServerLogger() {
    fclose(file);
}

void ServerLogger::log_start_transaction(int id, int n, char *client_machine_name, pid_t client_pid) {
    fprintf(file, OUTPUT_FORMAT_START, get_epoch_time(), id, n,
            get_client_name(client_machine_name, client_pid).c_str());
}

void ServerLogger::log_finish_transaction(int id, char *client_machine_name, pid_t client_pid) {
    fprintf(file, OUTPUT_FORMAT_FINISH, get_epoch_time(), id,
            get_client_name(client_machine_name, client_pid).c_str());
}

void ServerLogger::finish(int transaction_count, map<string, int> *client_transaction_map, double start_time,
                          double finish_time) {
    fprintf(file, "\nSUMMARY\n");

    // Print statistics for each client
    for (auto const&[client_name, client_transaction_count]: *client_transaction_map) {
        fprintf(file, OUTPUT_SUMMARY_FORMAT, client_transaction_count, client_name.c_str());
    }

    // Print transactions per second
    double total_time = finish_time - start_time;
    double rate = (double)transaction_count / total_time;
    fprintf(file, "%4.1f transactions/sec (%d/%.2f)\n", rate, transaction_count, total_time);
}
