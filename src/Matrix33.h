#pragma once

#include "Point3.h"

class Matrix33
{
public:
    Point3 x;
    Point3 y;
    Point3 z;

public:
    Matrix33(const Point3& x, const Point3& y, const Point3& z) : x(x), y(y), z(z) { }
    Matrix33(const Matrix33& m) : x(m.x), y(m.y), z(m.z) { }
};

Point3 operator*(const Point3& p, const Matrix33& m) {
    return Point3(p.x*m.x.x + p.y*m.x.y + p.z*m.x.z,
                  p.x*m.y.x + p.y*m.y.y + p.z*m.y.z,
                  p.x*m.z.x + p.y*m.z.y + p.z*m.z.z);
}
