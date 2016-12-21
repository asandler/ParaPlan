#include <algorithm>
#include <iostream>

#include "reach.h"

bool isFinState(const size_t edgeIndex, pair<double, double> borders, const SPDIReachTask reachTask) {
    auto it = reachTask.FinalEdgeParts.find(edgeIndex);

    if (it == reachTask.FinalEdgeParts.end()) {
        return false;
    } else {
        return ImagesIntersect(it->second, borders);
    }
}

pair<bool, pair<double, double> > IterateCycleAndCheckFinalState(
    const vector<size_t>& cycle,
    const pair<double, double>& borders,
    const SPDI& spdi,
    const SPDIReachTask reachTask
) {
    auto im = borders;

    for (size_t i = 0; i < cycle.size() - 1; ++i) {
        if (!ValidImage(im)) {
            return make_pair(false, make_pair(-1, -2));
        } else {

            if (isFinState(cycle[i], im, reachTask)) {
                return make_pair(true, im);
            }

            im = SuccInt(
                im.first,
                im.second,
                spdi.Edges[cycle[i]],
                spdi.Edges[cycle[i + 1]],
                spdi.EdgesConnections[cycle[i]].second.first,
                spdi.EdgesConnections[cycle[i]].second.second
            );
        }
    }

    if (ValidImage(im)) {
        return make_pair(isFinState(cycle.back(), im, reachTask), im);
    } else {
        return make_pair(false, make_pair(-1, -2));
    }
}

pair<bool, vector<pair<double, double> > > TestCycleAndGetFinalImages(
    const vector<size_t>& cycle,
    const pair<double, double>& borders,
    const SPDI& spdi,
    const SPDIReachTask reachTask
) {
    const auto badAnswer = make_pair(false, vector<pair<double, double> >(1, make_pair(-1, -2)));

    double left1 = SuccPointAMFSigma(borders.first, cycle, 'L', spdi);
    double left2 = SuccPointAMFSigma(borders.second, cycle, 'L', spdi);
    double leftA = (left2 - left1) / (borders.second - borders.first);
    double leftB = left1 - leftA * borders.first;
    double lim1L = CalcLimitPoint(leftA, leftB, borders.first);
    double lim1R = CalcLimitPoint(leftA, leftB, borders.second);

    double right1 = SuccPointAMFSigma(borders.first, cycle, 'R', spdi);
    double right2 = SuccPointAMFSigma(borders.second, cycle, 'R', spdi);
    double rightA = (right2 - right1) / (borders.second - borders.first);
    double rightB = right1 - rightA * borders.first;
    double lim2L = CalcLimitPoint(rightA, rightB, borders.first);
    double lim2R = CalcLimitPoint(rightA, rightB, borders.second);

    double leftLim = min(min(lim1L, lim1R), min(lim2L, lim2R));
    double rightLim = max(max(lim1L, lim1R), max(lim2L, lim2R));

    double imL = max(0.0, min(min(left1, left2), min(right1, right2)));
    double imR = min(1.0, max(max(left1, left2), max(right1, right2)));

    if (!ImagesIntersect(borders, make_pair(imL, imR))) {
        /*
            DIE cycle
        */
        //cout << "DIE-cycle" << endl;
        auto im = borders;
        auto answer = make_pair(false, vector<pair<double, double> >(1, im));

        while (ValidImage(im)) {
            const auto res = IterateCycleAndCheckFinalState(cycle, im, spdi, reachTask);

            answer.first |= res.first;
            answer.second.push_back(res.second);

            if (res.first) {
                return answer;
            }

            im = res.second;
        }
    } else {
        /*
            STAY, EXIT-LEFT, EXIT-RIGHT, EXIT-BOTH cycles
        */
        //cout << "Normal cycle" << endl;
        double lFin = max(0.0, min(min(borders.first, leftLim), min(borders.second, rightLim)));
        double rFin = min(1.0, max(max(borders.first, leftLim), max(borders.second, rightLim)));
        pair<double, double> finalDomain = make_pair(lFin, rFin);

        if (!ValidImage(finalDomain)) {
            return badAnswer;
        }

        const auto ans = IterateCycleAndCheckFinalState(cycle, finalDomain, spdi, reachTask);
        return make_pair(ans.first, vector<pair<double, double> >(1, finalDomain));
    }

    return badAnswer;
}

bool DFSSignaturesExploration(
    const size_t edgeIndex,
    const pair<double, double>& borders,
    const SPDI& spdi,
    const SPDIReachTask& reachTask,
    unordered_set<size_t> visitedEdges = unordered_set<size_t>(),
    unordered_set<string> visitedCycles = unordered_set<string>(),
    vector<size_t> curResidualPath = vector<size_t>()
) {
    //for (size_t i = 0; i < curResidualPath.size(); ++i) {
    //    cout << " ";
    //}
    //cout << edgeIndex << "\t" << spdi.EdgeIdMap[edgeIndex] << ", (" << borders.first << ":" << borders.second << ")" << endl;

    if (isFinState(edgeIndex, borders, reachTask)) {
        return true;
    }

    if (visitedEdges.find(edgeIndex) != visitedEdges.end()) {

        vector<size_t> cycle; //store all edges of cycle in reversed order
        bool inCycle = true;

        for (int i = curResidualPath.size() - 1; i >= 0; --i) {
            if (inCycle) {
                cycle.push_back(curResidualPath[i]);
                visitedEdges.erase(curResidualPath[i]);
            }

            if (curResidualPath[i] == edgeIndex) {
                inCycle = false;
            }
        }

        if (inCycle) {
            /*
                cycle ends not in current residual path, forbidden
            */
            return false;
        }

        vector<size_t> sortedCycle(cycle);
        sort(sortedCycle.begin(), sortedCycle.end());

        string cycleId = "";
        for (size_t i = 0; i < sortedCycle.size(); ++i) {
            cycleId += to_string(sortedCycle[i]) + "_";
        }

        if (visitedCycles.find(cycleId) != visitedCycles.end()) {
            return false;
        } else {
            //cout << "cycleId = " << cycleId << endl;
            visitedCycles.insert(cycleId);
        }

        reverse(cycle.begin(), cycle.end());    //restore original order of cycle edges
        cycle.push_back(edgeIndex);             //close cycle with the initial edge

        const auto res = TestCycleAndGetFinalImages(cycle, borders, spdi, reachTask);

        if (res.first) {
            return true;
        } else {
            for (const auto& im : res.second) {
                if (!ValidImage(im)) {
                    return false;
                }

                //cout << "Final image = " << im.first << " " << im.second << endl;

                vec2 c1 = spdi.EdgesConnections[edgeIndex].second.first;
                vec2 c2 = spdi.EdgesConnections[edgeIndex].second.second;

                for (const auto& nextEdge : spdi.EdgesConnections[edgeIndex].first) {
                    pair<double, double> image = SuccInt(im.first, im.second, spdi.Edges[edgeIndex], spdi.Edges[nextEdge], c1, c2);

                    if (ValidImage(image)) {
                        if (DFSSignaturesExploration(nextEdge, image, spdi, reachTask, visitedEdges, visitedCycles)) {
                            return true;
                        }
                    }
                }
            }
        }

    } else {

        visitedEdges.insert(edgeIndex);
        curResidualPath.push_back(edgeIndex);

        vec2 c1 = spdi.EdgesConnections[edgeIndex].second.first;
        vec2 c2 = spdi.EdgesConnections[edgeIndex].second.second;

        for (const auto& nextEdge : spdi.EdgesConnections[edgeIndex].first) {
            pair<double, double> image = SuccInt(borders.first, borders.second, spdi.Edges[edgeIndex], spdi.Edges[nextEdge], c1, c2);

            if (ValidImage(image)) {
                if (DFSSignaturesExploration(nextEdge, image, spdi, reachTask, visitedEdges, visitedCycles, curResidualPath)) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool SolveReachTask(const SPDI& spdi, const SPDIReachTask& reachTask) {
    for (const auto& startEdge : reachTask.StartEdgeParts) {
        if (DFSSignaturesExploration(startEdge.first, startEdge.second, spdi, reachTask)) {
            return true;
        }
    }

    return false;
}
