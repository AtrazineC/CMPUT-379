#include <iostream>
#include <string>
#include <vector>
#include <signal.h>

#include "util.h"
#include "command.h"

using namespace std;

int main() {
    // Handle signals
    handle_signals();

    // Handle input
    for (string line; cout << "shell379 > " && getline(cin, line); ) {
        if (!line.empty()) {
            // Run command
            run_command(line);
        }
    }

    return 0;
}
