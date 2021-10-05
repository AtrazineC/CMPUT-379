#include "command.h"

void print_resource_usage() {
    struct rusage usage{};
    getrusage(RUSAGE_CHILDREN, &usage);

    printf("User time = %7d seconds\n", (int) usage.ru_utime.tv_sec);
    printf("Sys  time = %7d seconds\n\n", (int) usage.ru_stime.tv_sec);
}

int get_resource_usage_for_pid(pid_t pid) {
    int process_time;
    char buffer[100];
    FILE *fp;

    sprintf(buffer, "ps -p %d -o times=", pid);
    fp = popen(buffer, "r");
    fscanf(fp, "%d", &process_time);
    pclose(fp);

    return process_time;
}

void command_exit() {
    // Clean up all the processes
    for (auto &it : *get_process_map()) {
        pid_t pid = it.second.pid;
        kill(pid, SIGCONT);
        kill(pid, SIGTERM);
        waitpid(pid, nullptr, 0);
        kill(pid, SIGKILL);
    }

    get_process_map()->clear();

    printf("\nResources used:\n");
    print_resource_usage();

    fflush(stdout);
    _exit(EXIT_SUCCESS);
}

void command_jobs() {
    printf("\nRunning processes:\n");

    // List out processes, process ids, etc.
    if (!get_process_map()->empty()) {
        printf(" #      PID S SEC COMMAND\n");
        int position = 0;

        for (auto &it : *get_process_map()) {
            Process process = it.second;

            // Get the resource usage of this process with pipe
            int process_time = get_resource_usage_for_pid(process.pid);

            // Print the process state
            printf(
                    "%2d: %7d %c %3d %s\n",
                    position,
                    process.pid,
                    process.get_status_code(),
                    process_time,
                    process.command.c_str());

            position++;
        }
    }

    printf("Processes = %7d active\n", (int) get_process_map()->size());
    printf("Completed processes:\n");

    print_resource_usage();
}

void command_kill(const vector<char *> &tokens) {
    if (tokens_contain_pid(tokens)) {
        pid_t pid = get_pid_from_tokens(tokens);
        Process *process = get_process(pid);

        if (process != nullptr) {
            kill(pid, SIGCONT); // Resume the process so it can be terminated
            kill(pid, SIGTERM);
            waitpid(pid, nullptr, 0);
            kill(pid, SIGKILL);
            get_process_map()->erase(pid);
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
            if (waitpid(get_pid_from_tokens(tokens), nullptr, 0) > 0) {
                get_process_map()->erase(pid);
            }
        } else {
            printf("Couldn't find process with pid %d\n", pid);
        }
    } else {
        printf("2nd argument (pid) missing for wait\n");
    }
}

bool check_tokens(const vector<char *> &tokens) {
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

void push_input_to_args(vector<char *> &args, char *file_name) {
    ifstream input((string) file_name);

    // Split each line and put into args
    string line;
    while (getline(input, line)) {
        vector<string> split_input = split(line);
        vector<char *> input_tokens = string_vector_to_char_vector(split_input);

        for (char *input_token : input_tokens) {
            args.push_back(input_token);
        }
    }
}

vector<char *> get_args(const vector<char *> &tokens) {
    vector<char *> args = vector<char *>();

    if (tokens.size() > 1) {
        for (int i = 1; i < tokens.size(); i++) {
            char *token = tokens[i];

            // Last token may be an ampersand
            // If it is, don't put it in args
            if ((i != tokens.size() - 1) || (strcmp(token, "&") != 0)) {
                // If input file, append input to current arg list
                if (token[0] == '<') {
                    push_input_to_args(args, trim_first_character(token));
                } else if (token[0] != '>') { // Don't append tokens for file output to args
                    args.push_back(token);
                }
            }
        }
    }

    return args;
}

char *get_output_file_name(const vector<char *>& tokens) {
    for (char *token : tokens) {
        if (token[0] == '>') {
            return trim_first_character(token);
        }
    }

    return nullptr;
}

bool check_if_background(vector<char *> tokens) {
    if (tokens.size() > 1) {
        char *last_token = tokens[tokens.size() - 1];
        return strcmp(last_token, "&") == 0;
    }

    return false;
}

void run_tokens(vector<char *> tokens, string line) {
    if (!tokens.empty()) {
        // Get command (first token)
        string command = tokens[0];

        // Get args
        vector<char *> args = get_args(tokens);

        // Check if output file (nullptr if no output file)
        char *output_file_name = get_output_file_name(tokens);

        // Get if background process
        bool is_background = check_if_background(tokens);

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
            new_process(command, args, output_file_name, is_background, line);
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
