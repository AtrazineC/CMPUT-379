#ifndef ASSIGNMENT1_PROCESS_H
#define ASSIGNMENT1_PROCESS_H

#include <unistd.h>
#include <cstring>
#include <map>
#include <wait.h>
#include <sys/resource.h>
#include <signal.h>

using namespace std;

class Process {
public:
    enum class Status {
        running,
        paused,
    };

    pid_t pid;
    string command;
    Status status;

    Process(pid_t pid, string command, Status status);
    char get_status_code();
};

#endif //ASSIGNMENT1_PROCESS_H
