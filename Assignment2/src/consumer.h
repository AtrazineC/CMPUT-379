#ifndef ASSIGNMENT2_CONSUMER_H
#define ASSIGNMENT2_CONSUMER_H

#include <pthread.h>

#include "command.h"
#include "shared_data.h"
#include "tands.h"

using namespace std;

/**
 * Consumer responsible for a thread
 */
class Consumer {
private:
    /**
     * Id of the consumer
     */
    int id;

    /**
     * Shared data between all consumers
     */
    SharedData *shared_data;

    /**
     * pthread id this consumer manages
     */
    pthread_t pthread_id;

    /**
     * Number of tasks this consumer has completed
     */
    int tasks_completed = 0;

    /**
     * Whether the consumer is exiting
     */
    bool closing = false;

    /**
     * Requests work. Returns false if exiting
     * @return false if exiting, true otherwise
     */
    bool request_work();

    /**
     * Perform the work associated with the given command
     * @param command The command to execute
     */
    void perform_work(Command *command);

    /**
     * Entry point once thread is spawned
     * @param args Pointer to consumer that wraps the thread
     * @return
     */
    static void *consume(void *args);

public:
    /**
     * Constructor
     * @param id Id of this consumer
     * @param shared_data Shared data
     */
    Consumer(int id, SharedData *shared_data);

    /**
     * Close the consumer
     */
    void close();

    /**
     * Get id of consumer
     * @return Id of consumer
     */
    int get_id();

    /**
     * Get pthread_id this consumer manages
     * @return pthread_id this consumer manages
     */
    pthread_t get_pthread_id();

    /**
     * Get number of tasks this consumer completed
     * @return number of tasks completed
     */
    int get_tasks_completed();
};

#endif //ASSIGNMENT2_CONSUMER_H
