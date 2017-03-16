#include <stdexcept>
#include <iostream>

#include "succ.h"

double SuccPointAMF(const double x, const Edge& e1, const Edge& e2, const vec2& c) {
    //cout << "SuccPointAMF: e1(start, dir) = (" << e1.Start.x << ", " << e1.Start.y << "), (" << e1.Dir.x << ", " << e1.Dir.y << ")" << endl;
    //cout << "SuccPointAMF: e2(start, dir) = (" << e2.Start.x << ", " << e2.Start.y << "), (" << e2.Dir.x << ", " << e2.Dir.y << ")" << endl;
    if (OrientedAngle(((e2.Start + e2.Dir * 0.5) - (e1.Start + e1.Dir * 0.5)), e2.Dir) * OrientedAngle(c, e2.Dir) < 0) {
        return -1;
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

    return alpha * x + beta;
}

double SuccPointTAMF(const double x, const Edge& e1, const Edge& e2, const vec2& c) {
    if (x < 0 || x > 1) {
        return -1;
    }

    double ans = SuccPointAMF(x, e1, e2, c);

    if (ans >= 0 && ans <= 1) {
        return ans;
    } else {
        return -1;
    }
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
            ans = SuccPointAMF(ans, spdi.Edges[sigma[i]], spdi.Edges[sigma[i + 1]], spdi.EdgesConnections[sigma[i]].second.first);
        } else if (type == 'L') {
            ans = SuccPointAMF(ans, spdi.Edges[sigma[i]], spdi.Edges[sigma[i + 1]], spdi.EdgesConnections[sigma[i]].second.second);
        } else {
            throw logic_error("Unknown successor type");
        }
        ans = min(1.0, ans);
        ans = max(0.0, ans);
    }

    return ans;
}

pair<double, double> SuccInt(const double x1, const double x2, const Edge& e1, const Edge& e2, const vec2& c1, const vec2& c2) {
    double x1Im1 = SuccPointAMF(x1, e1, e2, c1);
    double x1Im2 = SuccPointAMF(x1, e1, e2, c2);
    double x2Im1 = SuccPointAMF(x2, e1, e2, c1);
    double x2Im2 = SuccPointAMF(x2, e1, e2, c2);

    //cout << x1 << " " << x2 << endl;
    //cout << c1.x << " " << c1.y << " " << c2.x << " " << c2.y << endl;
    //cout << x1Im1 << " " << x1Im2 << " " << x2Im1 << " " << x2Im2 << endl;

    double lm = min(min(x1Im1, x1Im2), min(x2Im1, x2Im2));
    double rm = max(max(x1Im1, x1Im2), max(x2Im1, x2Im2));

    if ((lm > 1) || (rm < 0)) {
        return make_pair(-1, -2);
    }

    return make_pair(max(0.0, lm), min(1.0, rm));
}
