#pragma once

#ifndef VPoint_h
#define VPoint_h

#include <string>
#include <cmath>

struct VPoint {
public:
    VPoint() {
    }

    VPoint(double nx, double ny)
        : x(nx)
        , y(ny)
        , id("")
    {
    }

    VPoint(double nx, double ny, std::string sid)
        : x(nx)
        , y(ny)
        , id(sid)
    {
    }

    double Length() const {
        return sqrt(x * x + y * y);
    }

    double x;
    double y;
    std::string id;
};

#endif
