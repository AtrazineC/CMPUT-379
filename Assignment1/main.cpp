#include <iostream>
#include <string>

#include "command.h"
#include "signal_handler.h"

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
