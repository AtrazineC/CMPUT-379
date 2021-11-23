#include <iostream>

#include "task_manager.h"

#define OUTPUT_NAME_TEMPLATE "prodcon.%d.log"
#define OUTPUT_NAME_DEFAULT "prodcon.log"

using namespace std;

int main(int argc, char *argv[]) {
    // Process arguments
    int nthreads = 0;
    int output_id = 0;
    char output_name[sizeof(OUTPUT_NAME_TEMPLATE) + 10];

    // Check if minimum number of arguments was provided
    if (argc < 2) {
        fprintf(stderr, "Error: must include thread count.\n");
        return 0;
    }

    // Get thread count
    nthreads = atoi(argv[1]);

    // See if output id is given
    if (argc >= 3) {
        output_id = atoi(argv[2]);
    }

    // Get output file name
    if (output_id == 0) {
        sprintf(output_name, OUTPUT_NAME_DEFAULT);
    } else {
        sprintf(output_name, OUTPUT_NAME_TEMPLATE, output_id);
    }

    // Create task manager and run it
    TaskManager task_manager = TaskManager(nthreads, output_name);
    while (task_manager.run());

    return 0;
}
