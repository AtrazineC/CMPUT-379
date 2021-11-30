#include "client_logger.h"

ClientLogger::ClientLogger(int port, pid_t pid, char *host_name, char *server_ip_address) {
    // Create/open output file
    string output_name = get_client_name(host_name, pid);
    file = fopen(output_name.c_str(), "w");

    // Initial logging
    fprintf(file, "Using port %d\n", port);
    fprintf(file, "Using server address %s\n", server_ip_address);
    fprintf(file, "Host %s\n\n", output_name.c_str());
}

ClientLogger::~ClientLogger() {
    fclose(file);
}

void ClientLogger::log_transaction(int n) {
    transaction_count++;
    fprintf(file, OUTPUT_FORMAT_TRANSACTION, get_epoch_time(), "Send", "T", n);
}

void ClientLogger::log_sleep(int n) {
    fprintf(file, OUTPUT_FORMAT_SLEEP, n);
}

void ClientLogger::log_command(Command *command) {
    if (command->command_type == Command::CommandType::transaction) {
        log_transaction(command->n);
    } else if (command->command_type == Command::CommandType::sleep) {
        log_sleep(command->n);
    }
}

void ClientLogger::log_receive(int result_id) {
    fprintf(file, OUTPUT_FORMAT_RECEIVE, get_epoch_time(), "Recv", "D", result_id);
}

void ClientLogger::finish() {
    if (transaction_count == 1) {
        fprintf(file, "\nSent %d transaction\n", transaction_count);
    } else {
        fprintf(file, "\nSent %d transactions\n", transaction_count);
    }
}
