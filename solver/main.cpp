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
//    SPDIReachTask reachTask;

    ReadAndValidateSPDI(argv[1], spdi);
//    ReadStartAndFinalEdgeParts(argv[2], spdi, reachTask);
//
//    for (const auto& rec : reachTask.StartEdgeParts) {
//        cout << rec.first << " " << rec.second.first << " " << rec.second.second << endl;
//    }
//    cout << endl;
//    for (const auto& rec : reachTask.FinalEdgeParts) {
//        cout << rec.first << " " << rec.second.first << " " << rec.second.second << endl;
//    }
//
//    cout << SolveReachTask(spdi, reachTask) << endl;

    return 0;
}
