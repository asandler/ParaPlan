#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "spdi_io.h"

void ReadAndValidateSPDI(const string& filename, SPDI& spdi, bool strict) {
    unordered_map<string, vec2> Vertices;
    unordered_map<string, vec2> Vectors;
    unordered_map<size_t, unordered_set<size_t>> VisitedOutputEdges;

    string s;
    char state = 'n';
    size_t curRegionNumber = 0;

    ifstream spdiFile(filename);
    while (!spdiFile.eof()) {
        getline(spdiFile, s);

        if (s.length() == 0 || s[0] == '*') {
            continue;
        } else if (s == "Points:") {
            state = 'p';
            continue;
        } else if (s == "Vectors:") {
            state = 'v';
            continue;
        } else if (s == "Regions:") {
            state = 'r';
            continue;
        }

        switch (state) {
            case 'p': {
                string id = s.substr(0, s.find('.'));
                string coords = s.substr(id.length() + 2);
                double c1 = stod(coords.substr(0, coords.find(',')));
                double c2 = stod(coords.substr(coords.find(',') + 2));
                Vertices[id] = vec2(c1, c2);

                break;
            }

            case 'v': {
                string id = s.substr(0, s.find('.'));
                string coords = s.substr(id.length() + 2);
                double c1 = stod(coords.substr(0, coords.find(',')));
                double c2 = stod(coords.substr(coords.find(',') + 2));
                Vectors[id] = vec2(c1, c2);

                break;
            }

            case 'r': {
                vector<string> regionVertices;
                while (s.find(':') != string::npos) {
                    regionVertices.push_back(s.substr(0, s.find(':') - 1));
                    s = s.substr(s.find(':') + 2);
                }
                regionVertices.push_back(s.substr(0, s.find(',')));

                s = s.substr(s.find(',') + 2);
                string vecId1 = s.substr(0, s.find(','));
                string vecId2 = s.substr(vecId1.length() + 2);

                if (OrientedAngle(Vectors[vecId1], Vectors[vecId2]) < 0) {
                    string tmp = vecId1;
                    vecId1 = vecId2;
                    vecId2 = tmp;
                }

                for (size_t i = 0; i < regionVertices.size() - 1; ++i) {
                    string edgeId = ConstructEdgeName(regionVertices[i], regionVertices[i + 1]);

                    if (spdi.EdgeIdRemap.find(edgeId) == spdi.EdgeIdRemap.end()) {
                        spdi.Edges.push_back(Edge(Vertices[regionVertices[i]], Vertices[regionVertices[i + 1]]));
                        spdi.EdgeIdRemap[edgeId] = spdi.Edges.size() - 1;
                        spdi.EdgeIdMap.push_back(edgeId);
                    }
                }

                for (size_t i = 0; i < regionVertices.size() - 1; ++i) {
                    spdi.EdgesConnections.push_back(pair<vector<size_t>, pair<vec2, vec2>>());

                    for (size_t j = 0; j < regionVertices.size() - 1; ++j) {
                        if (i == j) {
                            continue;
                        }

                        size_t edgeNum1 = spdi.EdgeIdRemap[ConstructEdgeName(regionVertices[i], regionVertices[i + 1])];
                        size_t edgeNum2 = spdi.EdgeIdRemap[ConstructEdgeName(regionVertices[j], regionVertices[j + 1])];

                        pair<double, double> image = SuccInt(0, 1, spdi.Edges[edgeNum1], spdi.Edges[edgeNum2], Vectors[vecId1], Vectors[vecId2]);

                        if (ValidImage(image)) {
                            VisitedOutputEdges[edgeNum2].insert(curRegionNumber);
                            if (strict && VisitedOutputEdges[edgeNum2].size() > 1) {
                                throw logic_error("Dynamics collide on edge " + regionVertices[j] + "-" + regionVertices[j + 1]);
                            } else {
                                spdi.EdgesConnections[edgeNum1].first.push_back(edgeNum2);
                                spdi.EdgesConnections[edgeNum1].second = make_pair(Vectors[vecId1], Vectors[vecId2]);
                            }
                        }
                    }
                }

                curRegionNumber++;
                break;
            }
            default: {
                break;
            }
        }
    }

    spdiFile.close();
}

void ReadStartAndFinalEdgeParts(const string& filename, const SPDI& spdi, SPDIReachTask& reachTask) {
    string s;
    char state = 'n';

    ifstream taskFile(filename);

    while (!taskFile.eof()) {
        getline(taskFile, s);

        if (s.length() == 0 || s[0] == '*') {
            continue;
        } else if (s == "Start:") {
            state = 's';
            continue;
        } else if (s == "Final:") {
            state = 'f';
            continue;
        }

        istringstream ss(s);
        string v1, v2;
        double b1, b2;

        ss >> v1 >> v2 >> b1 >> b2;
        string edgeId = ConstructEdgeName(v1, v2);
        pair<double, double> borders = make_pair(b1, b2);

        const auto& it = spdi.EdgeIdRemap.find(edgeId);

        if (it != spdi.EdgeIdRemap.end()) {
            if (ValidImage(borders)) {
                if (state == 's') {
                    reachTask.StartEdgeParts[it->second] = borders;
                } else if (state == 'f') {
                    reachTask.FinalEdgeParts[it->second] = borders;
                }
            } else {
                throw logic_error("Error: invalid edge part [" + to_string(b1) + ":" + to_string(b2) + "] on edge " + v1 + "-" + v2);
            }
        } else {
            throw logic_error("Error: no edge between points " + v1 + " and " + v2);
        }
    }

    taskFile.close();
}
