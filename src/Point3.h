#pragma once

#include <math.h>

class Point3
{
public:
    double x;
    double y;
    double z;

public:
    // constructors
    Point3(const Point3& p) : x(p.x), y(p.y), z(p.z) {}
    Point3(double x, double y, double z) : x(x), y(y), z(z) {}
    Point3() : x(0.0), y(0.0), z(0.0) {}

    double getLength() const {
        return sqrt(dot(*this, *this));
    }

    Point3 getUnit() const {
        double length = getLength();
        return Point3(x / length, y / length, z / length);
    }

    // friend functions
    friend const Point3 operator+(const Point3& lhd, const Point3& rhd) {
        return Point3(lhd.x + rhd.x,
                      lhd.y + rhd.y,
                      lhd.z + rhd.z);
    }

    friend const Point3 operator-(const Point3& lhd, const Point3& rhd) {
        return Point3(lhd.x - rhd.x,
                      lhd.y - rhd.y,
                      lhd.z - rhd.z);
    }

    friend double dot(const Point3& lhd, const Point3& rhd) {
        return lhd.x*rhd.x + lhd.y*rhd.y + lhd.z*rhd.z;
    }

    friend const Point3 cross(const Point3& lhd, const Point3& rhd) {
        return Point3(lhd.y*rhd.z - lhd.z*rhd.y,
                      lhd.z*rhd.x - lhd.x*rhd.z,
                      lhd.x*rhd.y - lhd.y*rhd.x);
    }
};
