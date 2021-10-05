#ifndef ASSIGNMENT1_PROCESS_HANDLER_H
#define ASSIGNMENT1_PROCESS_HANDLER_H

#include <map>
#include <vector>
#include <fcntl.h>
#include <iostream>

#include "process.h"

#define MAX_PT_ENTRIES 32 // Max entries in the Process Table

using namespace std;

map<pid_t, Process> *get_process_map();
Process *get_process(pid_t pid);
void new_process(string command, vector<char *> args, char *output_file_name, bool is_background, string line);

#endif //ASSIGNMENT1_PROCESS_HANDLER_H
