#include "shared_data.h"

SharedData::SharedData(int nthreads, char *output_name) {
    // Initialize synchronization locks/conditions
    this->buffer_not_empty_cond = PTHREAD_COND_INITIALIZER;
    this->buffer_not_full_cond = PTHREAD_COND_INITIALIZER;
    this->output_file_lock = PTHREAD_MUTEX_INITIALIZER;
    this->task_manager_stats_lock = PTHREAD_MUTEX_INITIALIZER;
    this->buffer_lock = PTHREAD_MUTEX_INITIALIZER;

    // Get maximum buffer size
    this->max_buffer_size = 2 * nthreads;

    // Open output file
    this->output_file = fopen(output_name, "w");

    // Set up task manager stats
    this->task_manager_stats = {0, 0, 0, 0, 0};

    // Log start time
    this->start_time = high_resolution_clock::now();
}

SharedData::~SharedData() {
    // Close output file
    fclose(output_file);
}

pthread_cond_t *SharedData::get_buffer_not_empty_cond() {
    return &buffer_not_empty_cond;
}

pthread_cond_t *SharedData::get_buffer_not_full_cond() {
    return &buffer_not_full_cond;
}

pthread_mutex_t *SharedData::get_buffer_lock() {
    return &buffer_lock;
}

TaskManagerStats *SharedData::get_task_manager_stats() {
    return &task_manager_stats;
}

FILE *SharedData::get_output_file() {
    return output_file;
}

queue<Command> *SharedData::get_buffer() {
    return &buffer;
}

int SharedData::get_max_buffer_size() {
    return max_buffer_size;
}

void SharedData::log(int id, EventType event_type, Command *command) {
    log_to_output(id, event_type, command);
    log_to_task_manager_stats(event_type);
}

double SharedData::get_time() {
    return (double)duration_cast<microseconds>(high_resolution_clock::now() - start_time).count() / (double)1000000;
}

void SharedData::log_to_output(int id, EventType event_type, Command *command) {
    // Get time in seconds since task manager started
    double duration = get_time();

    // Get queue size (if applicable)
    char queue_size_str[] = "    ";
    if (event_type == EventType::receive || event_type == EventType::work) {
        sprintf(queue_size_str, "Q=%2u", get_buffer_size());
    }

    // Get event text
    char event_str[] = "          ";
    sprintf(event_str, "%s", event_type_to_string(event_type).c_str());

    // Get argument `n` (if applicable)
    char arg_str[] = "    ";
    if (command != nullptr) {
        sprintf(arg_str, "%4u", command->n);
    }

    // Lock output file
    pthread_mutex_lock(&output_file_lock);

    // Output to output file
    fprintf(output_file, OUTPUT_FORMAT, duration, id, queue_size_str, event_str, arg_str);

    // Free output file
    pthread_mutex_unlock(&output_file_lock);
}

void SharedData::log_to_task_manager_stats(EventType event_type) {
    // Lock task manager stats
    pthread_mutex_lock(&task_manager_stats_lock);

    // Increment corresponding event type
    switch (event_type) {
        case EventType::ask:
            task_manager_stats.ask_count++;
            break;
        case EventType::receive:
            task_manager_stats.receive_count++;
            break;
        case EventType::work:
            task_manager_stats.work_count++;
            break;
        case EventType::sleep:
            task_manager_stats.sleep_count++;
            break;
        case EventType::complete:
            task_manager_stats.complete_count++;
            break;
        default:
            break;
    }

    // Free task manager stats
    pthread_mutex_unlock(&task_manager_stats_lock);
}

int SharedData::get_buffer_size() {
    int size;

    // Lock buffer
    pthread_mutex_lock(&buffer_lock);

    // Access buffer size
    size = (int)buffer.size();

    // Free buffer
    pthread_mutex_unlock(&buffer_lock);

    return size;
}

string SharedData::event_type_to_string(EventType event_type) {
    switch (event_type) {
        case EventType::ask:
            return "Ask";
        case EventType::receive:
            return "Receive";
        case EventType::work:
            return "Work";
        case EventType::sleep:
            return "Sleep";
        case EventType::complete:
            return "Complete";
        case EventType::end:
            return "End";
        default:
            fprintf(stderr, "Error: Invalid event type.\n");
            return "???";
    }
}
