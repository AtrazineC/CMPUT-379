#ifndef ASSIGNMENT1_COMMAND_H
#define ASSIGNMENT1_COMMAND_H

#include <iostream>
#include <string>
#include <vector>

#include "util.h"

#define LINE_LENGTH 100 // Max # of characters in an input line
#define MAX_ARGS 7 // Max number of arguments to a command
#define MAX_LENGTH 20 // Max # of characters in an argument
#define MAX_PT_ENTRIES 32 // Max entries in the Process Table

using namespace std;

void run_command(string line);
void handle_signals();

#endif //ASSIGNMENT1_COMMAND_H
