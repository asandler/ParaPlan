#pragma once

#include <vector>

#include "helpers.h"

using namespace std;

pair<double, bool> SuccPointAMF(const double x, const Edge& e1, const Edge& e2, const vec2& c);

pair<double, double> SuccInt(
    const double x1,
    const double x2,
    const Edge& e1,
    const Edge& e2,
    const vec2& c1,
    const vec2& c2
);

double SuccPointAMFSigma(
    const double x,
    const vector<size_t>& sigma,
    const char type,
    const SPDI& spdi
);
