#include <stdexcept>
#include <iostream>

#include "succ.h"

pair <double, bool> SuccPointAMF(const double x, const Edge& e1, const Edge& e2, const vec2& c) {
    //cout << "SuccPointAMF: e1(start, dir) = (" << e1.Start.x << ", " << e1.Start.y << "), (" << e1.Dir.x << ", " << e1.Dir.y << ")" << endl;
    //cout << "SuccPointAMF: e2(start, dir) = (" << e2.Start.x << ", " << e2.Start.y << "), (" << e2.Dir.x << ", " << e2.Dir.y << ")" << endl;
    bool isReal = true;

    if (OrientedAngle(((e2.Start + e2.Dir * 0.5) - (e1.Start + e1.Dir * 0.5)), e2.Dir) * OrientedAngle(c, e2.Dir) < 0) {
        isReal = false;
    }

    vec2 cRot(c.y, -c.x);
    //cout << "SuccPointAMF: cRot = " << cRot.x << " " << cRot.y << endl;

    double prod1 = e1.Dir * cRot;
    double prod2 = e2.Dir * cRot;

    //cout << "SuccPointAMF: prod1 = " << prod1 << ", prod2 = " << prod2 << endl;

    if (fabs(prod2) < 0.0001) {
        throw logic_error("Vector field is parallel to edge");
    }

    double alpha = prod1 / prod2;
    double beta = ((e1.Start - e2.Start) * cRot) / prod2;

    return make_pair(alpha * x + beta, isReal);
}

pair<double, double> SuccInt(const double x1, const double x2, const Edge& e1, const Edge& e2, const vec2& c1, const vec2& c2) {
    vector<pair<double, bool> > pointImages;
    pointImages.push_back(SuccPointAMF(x1, e1, e2, c1));
    pointImages.push_back(SuccPointAMF(x1, e1, e2, c2));
    pointImages.push_back(SuccPointAMF(x2, e1, e2, c1));
    pointImages.push_back(SuccPointAMF(x2, e1, e2, c2));

    double minImaginaryPoint = 1e9;
    double maxRealPoint = -1e9;
    double minRealPoint = 1e9;
    bool hasReal = false;
    bool hasImaginary = false;

    for (const auto& im : pointImages) {
        if (im.second) {
            minRealPoint = min(minRealPoint, im.first);
            maxRealPoint = max(maxRealPoint, im.first);
            hasReal = true;
        } else {
            minImaginaryPoint = min(minImaginaryPoint, im.first);
            hasImaginary = true;
        }
    }

    if (hasImaginary) {
        if (!hasReal) {
            return make_pair(-1, -2);
        } else {
            if (minImaginaryPoint < maxRealPoint) {
                return make_pair(max(0.0, minRealPoint), 1.0);
            } else {
                return make_pair(0.0, min(1.0, maxRealPoint));
            }
        }
    }

    return make_pair(max(0.0, minRealPoint), min(1.0, maxRealPoint));
}

double SuccPointAMFSigma(
    const double x,
    const vector<size_t>& sigma,
    const char type,
    const SPDI& spdi
) {
    double ans = x;

    for (size_t i = 0; i < sigma.size() - 1; ++i) {
        if (type == 'R') { //right vector stored in first field of pair
            ans = SuccInt(ans, ans, spdi.Edges[sigma[i]], spdi.Edges[sigma[i + 1]], spdi.EdgesConnections[sigma[i]].second.first, spdi.EdgesConnections[sigma[i]].second.first).first;
        } else if (type == 'L') {
            ans = SuccInt(ans, ans, spdi.Edges[sigma[i]], spdi.Edges[sigma[i + 1]], spdi.EdgesConnections[sigma[i]].second.second, spdi.EdgesConnections[sigma[i]].second.second).first;
        } else {
            throw logic_error("Unknown successor type");
        }

        ans = min(1.0, ans);
        ans = max(0.0, ans);
    }

    return ans;
}
