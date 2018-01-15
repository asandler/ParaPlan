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
    cerr << "    --strict (optional): do not allow dynamics collision and parallel flow" << endl;
    cerr << "    --debug (optional): print debug information" << endl;
}

int main(int argc, char** argv) {
    char* spdiPath = NULL;
    char* spdiReachTaskPath = NULL;

    static struct option long_options[] = {
        {"spdi", required_argument, 0, 0},
        {"task", required_argument, 0, 0},
        {"threads", required_argument, 0, 0},
        {"strict", no_argument, 0, 0},
        {"debug", no_argument, 0, 0},
        {0, 0, 0, 0}
    };

    int option_index = 0, c = 0;

    while ((c = getopt_long(argc, argv, "h", long_options, &option_index)) != -1) {
        if (c == 0) {
            switch (option_index) {
                case 0:
                    spdiPath = optarg;
                    break;
                case 1:
                    spdiReachTaskPath = optarg;
                    break;
                case 2: 
                    ThreadsNumber = atoi(optarg);
                    FreeThreads = ThreadsNumber - 1;
                    break;
                case 3:
                    Strict = true;
                    break;
                case 4:
                    Debug = true;
                    break;
            }
        } else {
            PrintUsage();
            break;
        }
    }

    if (!spdiPath || !spdiReachTaskPath) {
        PrintUsage();
        return 1;
    }

    ReadAndValidateSPDI(spdiPath, Spdi, Strict);
    ReadStartAndFinalEdgeParts(spdiReachTaskPath, Spdi, ReachTask);

    pthread_mutex_init(&FreeThreadsMutex, NULL);
    pthread_mutex_init(&AnswerMutex, NULL);

    pthread_attr_init(&ThreadAttributes);
    pthread_attr_setdetachstate(&ThreadAttributes, PTHREAD_CREATE_JOINABLE);

    SolveReachTask(Spdi, ReachTask);

    pthread_mutex_destroy(&FreeThreadsMutex);
    pthread_mutex_destroy(&AnswerMutex);

    pthread_exit(NULL);
}
