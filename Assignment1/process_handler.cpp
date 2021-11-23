#include "process_handler.h"

inline map<pid_t, Process> process_map = map<pid_t, Process>();

map<pid_t, Process> *get_process_map() {
    return &process_map;
}

Process *get_process(pid_t pid) {
    if (process_map.find(pid) != process_map.end()) {
        return &process_map.at(pid);
    }

    return nullptr;
}

void new_process(string command, vector<char *> args, char *output_file_name, bool is_background, string line) {
    // Check we are under max Process count
    if (process_map.size() >= MAX_PT_ENTRIES) {
        printf("Maximum Process count exceeded\n");
        return;
    }

    // Fork
    pid_t pid = fork();

    if (pid < 0) {
        printf("Fork failed\n");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child

        // Convert to array of char *
        // (Arguments are all tokens except the first one)
        char *argv[args.size() + 1];
        for (int i = 0; i < args.size(); i++) {
            argv[i] = args[i];
        }
        argv[args.size()] = nullptr;

        // Check if redirect to output file
        if (output_file_name != nullptr) {
            freopen(output_file_name, "w", stdout);
        }

        if (execvp(argv[0], argv) < 0) {
            printf("execvp error\n");
            _exit(EXIT_FAILURE);
        }
    } else {
        // Parent

        // Add to unordered_map of processes
        Process process = Process(pid, line, Process::Status::running);
        process_map.emplace(pid, process);

        if (!is_background) {
            waitpid(pid, nullptr, 0);

            // Clean up stuff
            process_map.erase(pid);
        }
    }
}
