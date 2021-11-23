#ifndef ASSIGNMENT2_SHARED_DATA_H
#define ASSIGNMENT2_SHARED_DATA_H

#include <cstdio>
#include <queue>
#include <iostream>
#include <pthread.h>
#include <chrono>

#include "command.h"

#define OUTPUT_FORMAT "%7.3f ID=%2u %4s %-10s %5s\n"

using namespace std;
using namespace chrono;

/**
 * Struct for storing task manager summary
 */
struct TaskManagerStats {
    int work_count;
    int ask_count;
    int receive_count;
    int complete_count;
    int sleep_count;
    int yeet;
};

/**
 * Enum for event types
 */
enum class EventType {
    work,
    ask,
    receive,
    complete,
    sleep,
    end,
};

/**
 * Class for handling and storing shared data of task manager and consumers
 */
class SharedData {
private:
    /**
     * Conditions
     */
    pthread_cond_t buffer_not_empty_cond;
    pthread_cond_t buffer_not_full_cond;

    /**
     * Locks
     */
    pthread_mutex_t output_file_lock;
    pthread_mutex_t task_manager_stats_lock;
    pthread_mutex_t buffer_lock;

    /**
     * Statistics used for summary
     */
    TaskManagerStats task_manager_stats;

    /**
     * The output file
     */
    FILE *output_file;

    /**
     * The maximum buffer size
     */
    int max_buffer_size;

    /**
     * The buffer
     */
    queue<Command> buffer;

    /**
     * Start time of the task manager
     */
    time_point<system_clock, duration<double>> start_time;

    /**
     * Get current size of buffer
     * @return Current size of buffer
     */
    int get_buffer_size();

    /**
     * Log an event to output file
     * @param id Id of the thread
     * @param event_type Event type
     * @param command Optional command
     */
    void log_to_output(int id, EventType event_type, Command *command);

    /**
     * Log an event to task manager stats
     * @param event_type Event type
     */
    void log_to_task_manager_stats(EventType event_type);

    /**
     * Convert event type to string
     * @param event_type Event type to convert
     * @return
     */
    static string event_type_to_string(EventType event_type);

public:

    /**
     * Constructor
     * @param nthreads Number of consumers
     * @param output_name Name of output file
     */
    SharedData(int nthreads, char *output_name);

    /**
     * Destructor
     */
    ~SharedData();

    /**
     * Get buffer empty condition
     * @return Buffer empty condition
     */
    pthread_cond_t *get_buffer_not_empty_cond();

    /**
     * Get buffer full condition
     * @return Buffer full condition
     */
    pthread_cond_t *get_buffer_not_full_cond();

    /**
     * Get buffer lock
     * @return Buffer lock
     */
    pthread_mutex_t *get_buffer_lock();

    /**
     * Get task manager stats
     * @return Task manager stats
     */
    TaskManagerStats *get_task_manager_stats();

    /**
     * Get output file
     * @return The output file
     */
    FILE *get_output_file();

    /**
     * Get the shared buffer
     * @return The shared buffer
     */
    queue<Command> *get_buffer();

    /**
     * Get maximum buffer size
     * @return Maximum buffer size
     */
    int get_max_buffer_size();

    /**
     * Log an event
     * @param id Id of the thread
     * @param event_type Event type
     * @param command Optional command
     */
    void log(int id, EventType event_type, Command *command);

    /**
     * Get time in seconds since task manager started
     * @return Time in seconds since task manager started
     */
    double get_time();
};

#endif //ASSIGNMENT2_SHARED_DATA_H
