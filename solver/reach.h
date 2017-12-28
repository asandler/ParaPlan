#pragma once

#include <unordered_set>

#include "succ.h"
#include "helpers.h"
#include "globals.h"

using namespace std;

bool isFinState(const size_t edgeIndex, const pair<double, double>& borders, const SPDIReachTask& reachTask);

pair<bool, pair<double, double>> IterateCycleAndCheckFinalState(
    const vector<size_t>& cycle,
    const pair<double, double>& borders,
    const SPDI& spdi,
    const SPDIReachTask& reachTask
);

pair<bool, vector<pair<double, double>>> TestCycleAndGetFinalImages(
    const vector<size_t>& cycle,
    const pair<double, double>& borders,
    const SPDI& spdi,
    const SPDIReachTask& reachTask
);

void DFSSignaturesExploration(
    const size_t edgeIndex,
    const pair<double, double>& borders,
    vector<size_t> curResidualPath,
    unordered_set<size_t> visitedEdges,
    unordered_set<string> visitedCycles,
    const SPDI& spdi,
    const SPDIReachTask& spdiReachTask
);

void SolveReachTask(const SPDI& spdi, const SPDIReachTask& reachTask);
