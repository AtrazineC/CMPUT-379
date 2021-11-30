#ifndef ASSIGNMENT2_COMMAND_H
#define ASSIGNMENT2_COMMAND_H

#include <iostream>

#define TRANS_CMD 'T'
#define SLEEP_CMD 'S'

using namespace std;

/**
 * Struct for commands
 */
struct Command {
    /**
     * Enum for command types
     */
    enum class CommandType {
        transaction,
        sleep,
        close,
    };

    /**
     * Valid
     */
    bool valid = true;

    /**
     * Command type
     */
    CommandType command_type;

    /**
     * Argument
     */
    int n;
};

/**
 * Helper function for getting next command
 * @return Next command
 */
Command get_next_command();

#endif //ASSIGNMENT2_COMMAND_H
