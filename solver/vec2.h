#pragma once

#include <math.h>

class vec2 {
public:
    vec2() {
    }
    vec2(const vec2& _v)
        : x(_v.x)
        , y(_v.y)
    {
    }
    vec2(double _x, double _y)
        : x(_x)
        , y(_y)
    {
    }

    vec2 operator+(const vec2 b) const {
        return vec2(x + b.x, y + b.y);
    }

    vec2& operator+=(const vec2 b) {
        x += b.x;
        y += b.y;
        return *this;
    }

    vec2 operator-(const vec2 b) const {
        return vec2(x - b.x, y - b.y);
    }

    vec2& operator-=(const vec2 b) {
        x -= b.x;
        y -= b.y;
        return *this;
    }

    //scalar multiplication
    vec2 operator*(double lambda) const {
        return vec2(x * lambda, y * lambda);
    }

    vec2& operator*=(double lambda) {
        x *= lambda;
        y *= lambda;
        return *this;
    }

    //dot product
    double operator*(const vec2 b) const {
        return x * b.x + y * b.y;
    }

    double Length() const {
        return sqrt((*this) * (*this));
    }

    vec2& Normalize() {
        return (*this) *= 1.f / Length();
    }

    double x, y;
};
