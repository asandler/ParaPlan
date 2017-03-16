#include <iostream>

#include <getopt.h>
#include <stdlib.h>

#include "globals.h"
#include "helpers.h"
#include "spdi_io.h"
#include "succ.h"
#include "reach.h"

/*
    Andrei Sandler (andrei.sandler@phystech.edu), 2016-2017.
    SPDI solver.
    Everything is given in counter-clockwise order (points in region R, vectors of dynamics on R)
*/

using namespace std;

void PrintUsage() {
    cerr << "Usage:" << endl;
    cerr << "    --spdi path/to/spdi/file" << endl;
    cerr << "    --task path/to/reachability/task" << endl;
    cerr << "    --threads thread_number (optional, default = 1)" << endl;
}

int main(int argc, char** argv) {
    char* spdiPath = NULL;
    char* spdiReachTaskPath = NULL;

    static struct option long_options[] = {
        {"spdi", required_argument, 0, 0},
        {"task", required_argument, 0, 0},
        {"threads", required_argument, 0, 0},
        {0, 0, 0, 0}
    };

    int option_index = 0, c = 0;

    while ((c = getopt_long(argc, argv, "h", long_options, &option_index)) != -1) {
        switch (c) {
            case 0:
                if (option_index == 0) {
                    spdiPath = optarg;
                } else if (option_index == 1) {
                    spdiReachTaskPath = optarg;
                } else if (option_index == 2) {
                    ThreadsNumber = atoi(optarg);
                    FreeThreads = ThreadsNumber - 1;
                }
                break;
            default:
                PrintUsage();
                break;
        }
    }

    if (!spdiPath || !spdiReachTaskPath) {
        PrintUsage();
        return 1;
    }

    ReadAndValidateSPDI(spdiPath, Spdi);
    ReadStartAndFinalEdgeParts(spdiReachTaskPath, Spdi, ReachTask);

    pthread_mutex_init(&FreeThreadsMutex, NULL);
    pthread_mutex_init(&AnswerMutex, NULL);

    pthread_attr_init(&ThreadAttributes);
    pthread_attr_setdetachstate(&ThreadAttributes, PTHREAD_CREATE_DETACHED);

    SolveReachTask(Spdi, ReachTask);

    pthread_mutex_destroy(&FreeThreadsMutex);
    pthread_mutex_destroy(&AnswerMutex);

    pthread_exit(NULL);
}
