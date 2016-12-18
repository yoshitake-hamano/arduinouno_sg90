#pragma once

#include <math.h>

class Point2
{
public:
    double x;
    double y;

public:
    // constructors
    Point2(const Point2& p) : x(p.x), y(p.y) {}
    Point2(double x, double y) : x(x), y(y) {}
    Point2() : x(0.0), y(0.0) {}

    double getLength() const {
        return sqrt(dot(*this, *this));
    }

    Point2 getUnit() const {
        double length = getLength();
        return Point2(x / length, y / length);
    }

    double getAngle() const {
        return atan2(y, x);
    }

    // friend functions
    friend const Point2 operator+(const Point2& lhd, const Point2& rhd) {
        return Point2(lhd.x + rhd.x,
                      lhd.y + rhd.y);
    }

    friend const Point2 operator-(const Point2& lhd, const Point2& rhd) {
        return Point2(lhd.x - rhd.x,
                      lhd.y - rhd.y);
    }

    friend double dot(const Point2& lhd, const Point2& rhd) {
        return lhd.x*rhd.x + lhd.y*rhd.y;
    }
};
