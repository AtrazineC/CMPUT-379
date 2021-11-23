#ifndef ASSIGNMENT2_TASK_MANAGER_H
#define ASSIGNMENT2_TASK_MANAGER_H

#include <vector>

#include "shared_data.h"
#include "command.h"
#include "consumer.h"

using namespace std;

/**
 * Consumer manager
 */
class TaskManager {
private:
    /**
     * Id of task manager
     */
    static const int id = 0;

    /**
     * Shared data between tasks
     */
    SharedData *shared_data;

    /**
     * Holds all consumers
     */
    vector<Consumer *> consumers;

    /**
     * Execute a transaction
     * @param command The command where `command.n` is the length to simulate the execution of the transaction
     */
    void transaction(Command *command);

    /**
     * Simulate having the producer wait
     * @param command The command where `command.n` is the length in hundredths of a nanosecond to wait
     */
    void sleep(Command *command);

    /**
     * Close and clean up
     */
    void close();

    /**
     * Output the final summary to the output file
     */
    void output_summary();

public:
    /**
     * Constructor
     * @param nthreads Number of consumers
     * @param output_name Name of output file
     */
    TaskManager(int nthreads, char *output_name);

    /**
     * Destructor
     */
    ~TaskManager();

    /**
     * Run the task manager
     * @return true if continue, false if exit
     */
    bool run();
};

#endif //ASSIGNMENT2_TASK_MANAGER_H
