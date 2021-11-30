#include "util.h"

double get_epoch_time() {
    return (double)duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() / (double)1000;
}

string get_client_name(char *machine_name, pid_t pid) {
    return string(machine_name) + "." + to_string(pid);
}