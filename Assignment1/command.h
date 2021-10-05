#ifndef ASSIGNMENT1_COMMAND_H
#define ASSIGNMENT1_COMMAND_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <cstring>
#include <map>
#include <wait.h>
#include <sys/resource.h>
#include <csignal>

#include "util.h"
#include "process_handler.h"

#define LINE_LENGTH 100 // Max # of characters in an input line
#define MAX_ARGS 7 // Max number of arguments to a command
#define MAX_LENGTH 20 // Max # of characters in an argument

using namespace std;

void run_command(string line);

#endif //ASSIGNMENT1_COMMAND_H
