#include "consumer.h"

Consumer::Consumer(int id, SharedData *shared_data) {
    // Initialize values
    this->id = id;
    this->shared_data = shared_data;

    // Spawn thread (pass in consumer object as the arg)
    pthread_create(&pthread_id, nullptr, consume, this);
}

void Consumer::close() {
    closing = true;
}

int Consumer::get_id() {
    return id;
}

pthread_t Consumer::get_pthread_id() {
    return pthread_id;
}

int Consumer::get_tasks_completed() {
    return tasks_completed;
}

void *Consumer::consume(void *args) {
    Consumer *consumer = (Consumer *)args;
    while (consumer->request_work());

    return nullptr;
}

void Consumer::perform_work(Command *command) {
    Trans(command->n);
    shared_data->log(id, EventType::complete, command);
    tasks_completed++;
}

bool Consumer::request_work() {
    // Log this event
    shared_data->log(id, EventType::ask, nullptr);

    // Lock buffer
    pthread_mutex_lock(shared_data->get_buffer_lock());

    // Wait for work
    while (shared_data->get_buffer()->empty()) {
        // If task manager is closing, exit
        if (closing) {
            pthread_mutex_unlock(shared_data->get_buffer_lock());
            return false;
        }

        // Wait until signal that buffer is not empty
        pthread_cond_wait(shared_data->get_buffer_not_empty_cond(), shared_data->get_buffer_lock());
    }

    // Pop from queue
    Command command = shared_data->get_buffer()->front();
    shared_data->get_buffer()->pop();

    // Signal to producer that something was removed from the buffer
    pthread_cond_signal(shared_data->get_buffer_not_full_cond());

    // Unlock buffer
    pthread_mutex_unlock(shared_data->get_buffer_lock());

    // Log this event
    shared_data->log(id, EventType::receive, &command);

    // Perform the work
    perform_work(&command);

    return true;
}
