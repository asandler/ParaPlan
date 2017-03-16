#include <stdexcept>

#include "helpers.h"

double OrientedAngle(const vec2& a, const vec2& b) {
    double ans = acos((a * b) / (a.Length() * b.Length()));
    if (a.x * b.y - a.y * b.x < 0) {
        return -ans;
    }
    return ans;
}

bool ValidImage(const std::pair<double, double>& im) {
    return ((im.second - im.first > 0.0001) && im.first >= 0 && im.second <= 1 && im.second >= 0 && im.second <= 1);
}

std::string ConstructEdgeName(const std::string& s1, const std::string& s2) {
    if (s1 < s2) {
        return s1 + "_" + s2;
    } else {
        return s2 + "_" + s1;
    }
}

double CalcLimitPoint(const double a, const double b, const double x0) {
    if (a < 0) {
        throw std::logic_error("Some shit: a < 0");

    } else if (a < 1) {
        return b / (1 - a);

    } else if (a == 1) {

        if (b == 0) {
            return x0;
        } else if (b < 0) {
            return -inf;
        } else {
            return inf;
        }

    } else {

        if (x0 == (b / (1 - a))) {
            return x0;
        } else if (x0 > (b / (1 - a))) {
            return inf;
        } else {
            return -inf;
        }
    }
}

bool ImagesIntersect(const std::pair<double, double> a, const std::pair<double, double> b) {
    return (std::min(b.first, b.second) <= std::max(a.first, a.second)) && (std::max(b.first, b.second) >= std::min(a.first, a.second));
}
