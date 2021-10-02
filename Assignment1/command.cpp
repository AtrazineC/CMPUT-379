#include <unistd.h>
#include <cstring>
#include <map>
#include <wait.h>
#include <sys/resource.h>
#include <signal.h>

#include "command.h"

// TODO: move into separate file
class Process {
public:
    enum class Status {
        running,
        paused,
    };

    pid_t pid;
    string command;
    Status status;

    Process(pid_t pid, string command, Status status) {
        this->pid = pid;
        this->command = command;
        this->status = status;
    };

    char get_status_code() {
        if (status == Status::running) {
            return 'R';
        } else if (status == Status::paused) {
            return 'P';
        } else {
            return ' ';
        }
    }
};

map<pid_t, Process> process_map = map<pid_t, Process>();

Process *get_process(pid_t pid) {
    if (process_map.find(pid) != process_map.end()) {
        return &process_map.at(pid);
    }

    return nullptr;
}

void new_process(vector<char *> tokens, string line) {
    // Check we are under max Process count
    if (process_map.size() > MAX_PT_ENTRIES) {
        printf("Maximum Process count exceeded\n");
        return;
    }

    // Fork
    pid_t pid = fork();

    if (pid < 0) {
        printf("Fork failed\n");
        exit(1);
    }

    if (pid == 0) {
        // Child

        // Convert to array of char *
        char *argv[tokens.size() + 1];
        for (int i = 0; i < tokens.size(); i++) {
            argv[i] = tokens[i];
        }
        argv[tokens.size() + 1] = nullptr;

        // Execvp
        if (execvp(argv[0], argv) < 0) {
            printf("execvp error\n");
            _exit(0);
        }
    } else {
        // Check if background Process
        char *last_token = tokens[tokens.size() - 1];
        bool is_background = (strcmp(last_token, "&") == 0);

        // Add to unordered_map of processes
        Process process = Process(pid, line, Process::Status::running);
        process_map.emplace(pid, process);

        if (!is_background) {
            wait(nullptr);

            // Clean up stuff
            process_map.erase(pid);
        }
    }
}

void command_exit() {
    // Clean up all the processes
    for (auto &it : process_map) {
        pid_t pid = it.second.pid;
        kill(pid, SIGTERM);
        waitpid(pid, NULL, 0);
        kill(pid, SIGKILL);
    }

    fflush(stdout);
    _exit(0);
}

void command_jobs() {
    printf("\nRunning processes:\n");

    // List out processes, process ids, etc.
    if (!process_map.empty()) {
        printf(" #    PID S SEC COMMAND\n");
        int position = 0;

        for (auto &it : process_map) {
            Process process = it.second;

            // Print the process state
            printf(
                    "%2u: %5u %c %3u %s\n",
                    position,
                    process.pid,
                    process.get_status_code(),
                    0,
                    process.command.c_str());

            position++;
        }
    }

    printf("Processes = %d active\n", (int) process_map.size());
    printf("Completed processes:\n");

    struct rusage usage{};
    getrusage(RUSAGE_SELF, &usage);

    printf("User time = %7d seconds\n", (int) usage.ru_utime.tv_sec);
    printf("Sys  time = %7d seconds\n\n", (int) usage.ru_stime.tv_sec);
}

void command_kill(const vector<char *> &tokens) {
    if (tokens_contain_pid(tokens)) {
        pid_t pid = get_pid_from_tokens(tokens);
        Process *process = get_process(pid);

        if (process != nullptr) {
            kill(pid, SIGTERM);
            waitpid(pid, NULL, 0);
            kill(pid, SIGKILL);
            process_map.erase(pid);
        } else {
            printf("Couldn't find process with pid %d\n", pid);
        }
    } else {
        printf("2nd argument (pid) missing for kill\n");
    }
}

void command_resume(const vector<char *> &tokens) {
    if (tokens_contain_pid(tokens)) {
        pid_t pid = get_pid_from_tokens(tokens);
        Process *process = get_process(pid);

        if (process != nullptr) {
            kill(pid, SIGCONT);
            process->status = Process::Status::running;
        } else {
            printf("Couldn't find process with pid %d\n", pid);
        }
    } else {
        printf("2nd argument (pid) missing for resume\n");
    }
}

void command_sleep(const vector<char *> &tokens) {
    if (tokens.size() < 2) {
        printf("2nd argument (time) missing for sleep\n");
        return;
    }

    sleep(atoi(tokens[1]));
}

void command_suspend(const vector<char *> &tokens) {
    if (tokens_contain_pid(tokens)) {
        pid_t pid = get_pid_from_tokens(tokens);
        Process *process = get_process(pid);

        if (process != nullptr) {
            kill(pid, SIGSTOP);
            process->status = Process::Status::paused;
        } else {
            printf("Couldn't find process with pid %d\n", pid);
        }
    } else {
        printf("2nd argument (pid) missing for suspend\n");
    }
}

void command_wait(const vector<char *> &tokens) {
    if (tokens_contain_pid(tokens)) {
        pid_t pid = get_pid_from_tokens(tokens);
        Process *process = get_process(pid);

        if (process != nullptr) {
            waitpid(get_pid_from_tokens(tokens), nullptr, WNOHANG);
        } else {
            printf("Couldn't find process with pid %d\n", pid);
        }
    } else {
        printf("2nd argument (pid) missing for wait\n");
    }
}

bool check_tokens(vector<char *> tokens) {
    if (tokens.size() > MAX_ARGS) {
        printf("Too many arguments (maximum %d)\n", MAX_ARGS);
        return false;
    }

    for (int i = 0; i < tokens.size(); i++) {
        char *token = tokens[i];

        if (strlen(token) > MAX_LENGTH) {
            printf("Argument %d exceeds maximum length of %d\n", i + 1, MAX_LENGTH);
            return false;
        }
    }

    return true;
}

void run_tokens(vector<char *> tokens, string line) {
    if (!tokens.empty()) {
        // Get command (first token)
        string command = tokens[0];

        // Process command
        if (command == "exit") {
            command_exit();
        } else if (command == "jobs") {
            command_jobs();
        } else if (command == "kill") {
            command_kill(tokens);
        } else if (command == "resume") {
            command_resume(tokens);
        } else if (command == "sleep") {
            command_sleep(tokens);
        } else if (command == "suspend") {
            command_suspend(tokens);
        } else if (command == "wait") {
            command_wait(tokens);
        } else if (!command.empty()) {
            new_process(tokens, line);
        }
    }
}

void run_command(string line) {
    // Check maximum line length
    if (line.length() > LINE_LENGTH) {
        printf("Maximum line length of %d exceeded\n", LINE_LENGTH);
        return;
    }

    // Trim trailing whitespace
    line = rtrim(line);

    // Split into tokens and convert strings to char
    vector<string> split_input = split(line);
    vector<char *> tokens = string_vector_to_char_vector(split_input);

    // Check max argument and argument length max
    if (check_tokens(tokens)) {
        // Run tokens
        run_tokens(tokens, line);
    }
}

void signal_handler(int signum) {
    fprintf(stdout, "\n");

    // Detect when a child process has ended for cleanup
    if (signum == SIGCHLD) {

    }
}

void handle_signals() {
    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = signal_handler;
    sigaction(SIGINT, &sa, nullptr);
    sigaction(SIGTSTP, &sa, nullptr);
    sigaction(SIGCHLD, &sa, nullptr);
}

