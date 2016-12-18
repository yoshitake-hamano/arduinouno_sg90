#pragma once

class Double
{
public:
    static bool isZero(double d) {
        const double NEAR_MAX_VALUE =  0.001;
        const double NEAR_MIN_VALUE = -0.001;
        return ((NEAR_MIN_VALUE < d) && (d < NEAR_MAX_VALUE));
    }

    static bool isEquals(double lhd, double rhd) {
        double d = lhd - rhd;
        return isZero(d);
    }
};