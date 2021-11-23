#include "task_manager.h"

TaskManager::TaskManager(int nthreads, char *output_name) {
    // New shared data class
    this->shared_data = new SharedData(nthreads, output_name);

    // Spawn consumers
    for (int i = 0; i < nthreads; i++) {
        this->consumers.emplace_back(new Consumer(i + 1, this->shared_data));
    }
}

TaskManager::~TaskManager() {
    // Clean up consumers
    for (Consumer *consumer: consumers) {
        delete consumer;
    }

    consumers.clear();

    // Clean up shared data
    delete this->shared_data;
}

bool TaskManager::run() {
    Command command = get_next_command();

    if (command.valid) {
        // Perform the command
        if (command.command_type == Command::CommandType::transaction) {
            this->transaction(&command);
        } else if (command.command_type == Command::CommandType::sleep) {
            this->sleep(&command);
        } else if (command.command_type == Command::CommandType::close) {
            this->close();
            return false;
        }
    }

    return true;
}

void TaskManager::transaction(Command *command) {
    // Log this event
    shared_data->log(id, EventType::work, command);

    // Lock buffer
    pthread_mutex_lock(shared_data->get_buffer_lock());

    // Wait until queue isn't full
    while ((int)shared_data->get_buffer()->size() >= shared_data->get_max_buffer_size()) {
        // Wait for signal that something got removed from the buffer
        pthread_cond_wait(shared_data->get_buffer_not_full_cond(), shared_data->get_buffer_lock());
    }

    // Push to queue
    shared_data->get_buffer()->push(*command);

    // Signal to a thread that the buffer is no longer empty
    pthread_cond_signal(shared_data->get_buffer_not_empty_cond());

    // Free buffer
    pthread_mutex_unlock(shared_data->get_buffer_lock());
}

void TaskManager::sleep(Command *command) {
    // Log this event
    shared_data->log(id, EventType::sleep, command);

    // Sleep
    Sleep(command->n);
}

void TaskManager::close() {
    // Log this event
    shared_data->log(id, EventType::end, nullptr);

    // Lock buffer
    pthread_mutex_lock(shared_data->get_buffer_lock());

    // Notify each consumer to close
    for (Consumer *consumer: consumers) {
        consumer->close();
    }

    // Unblock threads waiting for work
    pthread_cond_broadcast(shared_data->get_buffer_not_empty_cond());

    // Unlock buffer
    pthread_mutex_unlock(shared_data->get_buffer_lock());

    // Wait for all threads to close
    for (Consumer *consumer: consumers) {
        pthread_join(consumer->get_pthread_id(), nullptr);
    }

    // Output final summary
    output_summary();
}

void TaskManager::output_summary() {
    FILE *output_file = shared_data->get_output_file();
    TaskManagerStats *task_manager_stats = shared_data->get_task_manager_stats();

    // Output task manager stats
    fprintf(output_file, "Summary:\n");
    fprintf(output_file, "    %-12s %5d\n", "Work", task_manager_stats->work_count);
    fprintf(output_file, "    %-12s %5d\n", "Ask", task_manager_stats->ask_count);
    fprintf(output_file, "    %-12s %5d\n", "Receive", task_manager_stats->receive_count);
    fprintf(output_file, "    %-12s %5d\n", "Complete", task_manager_stats->complete_count);
    fprintf(output_file, "    %-12s %5d\n", "Sleep", task_manager_stats->sleep_count);

    // Output thread stats
    for (Consumer *consumer: consumers) {
        fprintf(output_file, "    Thread %-3d %7d\n", consumer->get_id(), consumer->get_tasks_completed());
    }

    // Output transactions per second
    double duration = shared_data->get_time();
    double transactions_per_second = (double)task_manager_stats->complete_count / duration;
    fprintf(output_file, "Transactions per second: %-5.2f", transactions_per_second);
}
