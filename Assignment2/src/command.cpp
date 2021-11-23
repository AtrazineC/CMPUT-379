#include "command.h"

Command get_next_command() {
    Command command{};

    // Parse input
    char command_type;
    bool eof = (scanf("%c%u", &command_type, &command.n) == -1);

    if (!eof) {
        // Set command type
        if (command_type == TRANS_CMD) {
            command.command_type = Command::CommandType::transaction;
        } else if (command_type == SLEEP_CMD) {
            command.command_type = Command::CommandType::sleep;
        } else {
            command.valid = false;
        }
    } else {
        command.command_type = Command::CommandType::close;
    }

    return command;
}