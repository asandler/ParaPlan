#include <iostream>

#include "helpers.h"
#include "spdi_io.h"
#include "succ.h"
#include "reach.h"

/*
    Andrei Sandler (andrei.sandler@phystech.edu), 2016.
    SPDI solver.
    Everything is given in counter-clockwise order (points in region R, vectors of dynamics on R)
*/

using namespace std;

int main(int argc, char** argv) {
    SPDI spdi;
    SPDIReachTask reachTask;

    if (argc < 3) {
        cerr << "Arguments: .spdi file, .task file" << endl;
        return 1;
    }

    ReadAndValidateSPDI(argv[1], spdi);
    ReadStartAndFinalEdgeParts(argv[2], spdi, reachTask);

    cout << SolveReachTask(spdi, reachTask) << endl;

    return 0;
}
