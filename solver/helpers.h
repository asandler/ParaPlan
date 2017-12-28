#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "vec2.h"

#define inf 1000000

struct Edge {
    Edge(const vec2& s, const vec2& e)
        : Start(s)
        , Dir(e - s)
        {}

    vec2 Start;
    vec2 Dir;
};

struct SPDI {
    std::vector<Edge> Edges;
    std::vector<std::pair<std::vector<size_t>, std::pair<vec2, vec2> > > EdgesConnections;
    std::unordered_map<std::string, size_t> EdgeIdRemap;
    std::vector<std::string> EdgeIdMap;
};

struct SPDIReachTask {
    std::unordered_map<size_t, std::pair<double, double> > StartEdgeParts;
    std::unordered_map<size_t, std::pair<double, double> > FinalEdgeParts;
};

double OrientedAngle(const vec2& a, const vec2& b);

bool ValidImage(const std::pair<double, double>& im);

std::string ConstructEdgeName(const std::string& s1, const std::string& s2);

double CalcLimitPoint(const double a, const double b, const double x0);

bool ImagesIntersect(const std::pair<double, double> a, const std::pair<double, double> b);
