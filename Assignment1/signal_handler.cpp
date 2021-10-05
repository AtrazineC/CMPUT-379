#include "signal_handler.h"

void handle_signals() {
    // Capture interrupt
    signal(SIGINT, [](int signum) {
        pid_t pid = waitpid(-1, nullptr, WNOHANG);

        if (pid > 0) {
            get_process_map()->erase(pid);
        }

        fprintf(stdout, "\n");
    });

    // Ignore stop interrupt
    signal(SIGTSTP, SIG_IGN);

    // Capture child interrupts
    signal(SIGCHLD, [](int signum) {
        pid_t pid = waitpid(-1, nullptr, WNOHANG);
        get_process_map()->erase(pid);
    });
}
