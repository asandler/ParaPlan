#include <algorithm>
#include <iostream>
#include <cmath>
#include <time.h>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <sys/time.h>

#include "Voronoi.h"
#include "VPoint.h"

using namespace std;
using namespace vor;

double OrientedAngle(const VPoint& a, const VPoint& b) {
    double ans = acos((a.x * b.x + a.y * b.y) / (a.Length() * b.Length()));
    if (a.x * b.y - a.y * b.x < 0) {
        return -ans;
    }
    return ans;
}

bool PointIsValid(const VPoint& a) {
    return (a.x > 0 && a.x < 1000 && a.y > 0 && a.y < 1000);
}

string ConstructEdgeName(const string& a, const string& b) {
    if (a < b) {
        return a + "-" + b;
    } else {
        return b + "-" + a;
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        //cerr << "Argument missing: number of initial region centers" << endl;
        return 1;
    }

    Voronoi* v = new Voronoi();
    Vertices* ver = new Vertices();
    Vertices* dir = new Vertices();

    struct timeval time;
    gettimeofday(&time, NULL);
    srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

    double w = 1000;
    for (int i = 0; i < stoi(argv[1]); i++) {
        ver->push_back(new VPoint(w * (double)rand() / RAND_MAX, w * (double)rand() / RAND_MAX));
        dir->push_back(new VPoint((double)rand() / RAND_MAX - 0.5, (double)rand() / RAND_MAX - 0.5));
    }

    vor::Edges* edges = v->GetEdges(ver, w, w);

    unordered_map<string, VPoint> vertices;
    unordered_map<string, size_t> verticesNamesRemap;
    unordered_map<string, VPoint> verticesToOutput;

    unordered_map<string, vector<pair<VPoint, VPoint> > > regions;
    unordered_map<string, pair<VPoint, VPoint> > regionVectors;
    unordered_map<string, bool> outputEdges;

    for (vor::Edges::iterator i = edges->begin(); i != edges->end(); ++i) {
        double sx = (*i)->start->x;
        double sy = (*i)->start->y;
        double ex = (*i)->end->x;
        double ey = (*i)->end->y;

        double lx = (*i)->left->x;
        double ly = (*i)->left->y;
        double rx = (*i)->right->x;
        double ry = (*i)->right->y;

        string sid = to_string((int)sx) + "_" + to_string((int)sy);
        string eid = to_string((int)ex) + "_" + to_string((int)ey);
        string lid = to_string((int)lx) + "_" + to_string((int)ly);
        string rid = to_string((int)rx) + "_" + to_string((int)ry);

        if (vertices.find(sid) == vertices.end()) {
            vertices[sid] = VPoint(sx, sy, sid);
            verticesNamesRemap[sid] = verticesNamesRemap.size();
        }
        if (vertices.find(eid) == vertices.end()) {
            vertices[eid] = VPoint(ex, ey, eid);
            verticesNamesRemap[eid] = verticesNamesRemap.size();
        }
        if (regions.find(lid) == regions.end()) {
            regions[lid] = vector<pair<VPoint, VPoint> >();
        }
        if (regions.find(rid) == regions.end()) {
            regions[rid] = vector<pair<VPoint, VPoint> >();
        }
        
        regions[lid].push_back(make_pair(vertices[sid], vertices[eid]));
        regions[rid].push_back(make_pair(vertices[sid], vertices[eid]));
    }

    for (auto& r : regions) {
        bool goodRegion = true;
        for (const auto& v : r.second) {
            goodRegion &= PointIsValid(v.first);
            goodRegion &= PointIsValid(v.second);
        }

        if (!goodRegion) {
            regions.erase(r.first);
            continue;
        } else {
            for (const auto& v : r.second) {
                verticesToOutput[v.first.id] = v.first;
                verticesToOutput[v.second.id] = v.second;
            }
        }
    }

    for (auto& r : regions) {
        /*
            Sort edges in a region: A-B, B-C, C-D, ..., X-A
        */

        for (size_t i = 0; i < r.second.size() - 1; ++i) {
            for (size_t j = i + 1; j < r.second.size(); ++j) {
                if ((r.second[i].second.id == r.second[j].first.id) || (r.second[i].second.id == r.second[j].second.id)) {
                    //insert necessary edge next to current
                    pair<VPoint, VPoint> tmp = r.second[i + 1];
                    r.second[i + 1] = r.second[j];
                    r.second[j] = tmp;

                    //A-B, C-B,... => A-B, B-C,...
                    if (r.second[i].second.id == r.second[i + 1].second.id) {
                        VPoint tmp = r.second[i + 1].first;
                        r.second[i + 1].first = r.second[i + 1].second;
                        r.second[i + 1].second = tmp;
                    }

                    break;
                }
            }
        }

        /*
            Check for counter-clockwise orientation 
        */

        VPoint p0 = r.second[0].first;
        VPoint p1 = r.second[0].second;
        VPoint p2 = r.second[1].second;

        if (OrientedAngle(VPoint(p1.x - p0.x, p1.y - p0.y), VPoint(p2.x - p1.x, p2.y - p1.y)) < 0) {
            for (size_t i = 0; i < r.second.size(); ++i) {
                VPoint tmp = r.second[i].first;
                r.second[i].first = r.second[i].second;
                r.second[i].second = tmp;
            }
        }
    }

    for (const auto& r : regions) {
        //cerr << "Region: " << r.first << endl;
        vector<size_t> startPoints;

        for (size_t i = 0; i < r.second.size(); ++i) {
            string curEdgeId = ConstructEdgeName(r.second[i].first.id, r.second[i].second.id);
            //cerr << verticesNamesRemap[r.second[i].first.id] << "-" << verticesNamesRemap[r.second[i].second.id] << " " << curEdgeId << "\t" << outputEdges[curEdgeId] << endl;

            size_t i_next = (i + 1) % r.second.size();
            string nextEdgeId = ConstructEdgeName(r.second[i_next].first.id, r.second[i_next].second.id);

            if ((outputEdges[curEdgeId]) && (!outputEdges[nextEdgeId])) {
                startPoints.push_back(i_next);
            }
        }

        if (startPoints.size() == 0) {
            startPoints.push_back(rand() % r.second.size());
        }

        //cerr << "Start points" << endl;
        for (const auto& sp : startPoints) {
            //cerr << sp << endl;
        }

        for (auto startEdge : startPoints) {

            size_t prevEdge = (startEdge + r.second.size() - 1) % r.second.size();
            size_t postEdge = (startEdge + r.second.size() + 1) % r.second.size();
            size_t curEdge = startEdge;

            //cerr << "prevEdge = " << prevEdge << ", postEdge = " << postEdge << endl;

            VPoint first = r.second[startEdge].first;
            VPoint preFirst = r.second[prevEdge].first;

            VPoint last = r.second[startEdge].second;
            VPoint postLast = r.second[postEdge].second;

            VPoint leftVector(first.x - preFirst.x, first.y - preFirst.y);
            VPoint rightVector(last.x - postLast.x, last.y - postLast.y);

            //cerr << "preFirst = " << verticesNamesRemap[preFirst.id] << endl;
            //cerr << "first = " << verticesNamesRemap[first.id] << endl;
            //cerr << "last = " << verticesNamesRemap[last.id] << endl;
            //cerr << "postLast = " << verticesNamesRemap[postLast.id] << endl;

            bool foundAnswer = false;

            //cerr << "Start cycling" << endl << verticesNamesRemap[r.second[startEdge].first.id] << "-" << verticesNamesRemap[r.second[startEdge].second.id] << endl;

            while (postEdge != startEdge) {
                string edgeId = ConstructEdgeName(r.second[curEdge].first.id, r.second[curEdge].second.id);
                //cerr << verticesNamesRemap[r.second[curEdge].first.id] << "-" << verticesNamesRemap[r.second[curEdge].second.id] << " " << edgeId << endl;
                
                if (outputEdges[edgeId]) {
                    break;
                } else {
                    outputEdges[edgeId] = true;
                }

                //cerr << "OrientedAngle = " << OrientedAngle(leftVector, rightVector) << endl;
                if (OrientedAngle(leftVector, rightVector) > 0) {
                    foundAnswer = true;
                    break;
                }

                curEdge = postEdge;
                postEdge = (postEdge + r.second.size() + 1) % r.second.size();
                //cerr << "postEdge = " << postEdge << endl;

                last = postLast;
                postLast = r.second[postEdge].second;

                //cerr << "last = " << verticesNamesRemap[last.id] << endl;
                //cerr << "postLast = " << verticesNamesRemap[postLast.id] << endl;

                rightVector.x = last.x - postLast.x;
                rightVector.y = last.y - postLast.y;
            }

            if (foundAnswer) {
                double alpha1 = rand() * 1.0 / RAND_MAX;
                double alpha2 = rand() * 1.0 / RAND_MAX;

                VPoint vec1(leftVector.x * alpha1 + rightVector.x * (1 - alpha1), leftVector.y * alpha1 + rightVector.y * (1 - alpha1));
                VPoint vec2(leftVector.x * alpha2 + rightVector.x * (1 - alpha2), leftVector.y * alpha2 + rightVector.y * (1 - alpha2));

                regionVectors[r.first] = make_pair(vec1, vec2);

                break;
            } else {
                //regionVectors[r.first] = make_pair(VPoint(0, 0), VPoint(0, 0));
                //cout << "No luck :(" << endl;
                return 1;
            }
        }
    }

    cout << "Points:" << endl;

    for (const auto& v : verticesToOutput) {
        cout << verticesNamesRemap[v.second.id] << ". " << v.second.x << ", " << v.second.y << endl;
    }

    cout << endl << "Vectors:" << endl;

    for (const auto& r : regions) {
        cout << "v_" << r.first << "_1. " << regionVectors[r.first].first.x << ", " << regionVectors[r.first].first.y << endl;
        cout << "v_" << r.first << "_2. " << regionVectors[r.first].second.x << ", " << regionVectors[r.first].second.y << endl;
    }

    cout << endl << "Regions:" << endl;

    for (const auto& r : regions) {
        cout << "* R_" << r.first << endl;
        for (size_t i = 0; i < r.second.size(); ++i) {
            cout << verticesNamesRemap[r.second[i].first.id] << " : ";
        }
        cout << verticesNamesRemap[r.second[0].first.id] << ", v_" << r.first << "_1, v_" << r.first << "_2" << endl << endl;
    }

    delete v;
    delete ver;
    delete dir;
    
    return 0;
}
