#ifndef ASSIGNMENT3_UTIL_H
#define ASSIGNMENT3_UTIL_H

#include <cstdio>
#include <cstring>
#include <chrono>
#include <string>

using namespace std;
using namespace chrono;

/**
 * Get UNIX Epoch time
 * @return UNIX Epoch time
 */
double get_epoch_time();

/**
 * Get client name from machine name and pid
 * @return Client name
 */
string get_client_name(char *machine_name, pid_t pid);

#endif //ASSIGNMENT3_UTIL_H
