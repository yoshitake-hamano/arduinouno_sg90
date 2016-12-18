#pragma once

#include "Point2.h"
#include "Point3.h"

const static double LENGTH_OF_LOWER_ARM = 99.5;
const static double LENGTH_OF_UPPER_ARM = 102.5;

class ActuatorController
{
public:
    /**
     * Get the joint point.
     *
     * @param[in]  p     the end point on the robot arm.
     * @param[out] joint the joint point between the upper arm and lower arm.
     * @retval     true  the robot arm can reach the point.
     * @retval     false the robot arm can not reach the point.
     */
    static bool getJoint(const Point2& p, Point2 *joint) {
        Point2 origin;
        double length = p.getLength();
        if (length > (LENGTH_OF_LOWER_ARM + LENGTH_OF_UPPER_ARM)) {
            return false;
        }
        if (length < (LENGTH_OF_UPPER_ARM - LENGTH_OF_LOWER_ARM)) {
            return false;
        }

        double dot_p = dot(p, p);
        double dot_a = pow(LENGTH_OF_LOWER_ARM, 2);
        double dot_b = pow(LENGTH_OF_UPPER_ARM, 2);
        double k     = (dot_p + dot_a - dot_b) / 2.0;
        joint->x     = (k * p.x - p.y * sqrt(dot_p * dot_a - pow(k, 2))) / dot_p;
        joint->y     = (k * p.y + p.x * sqrt(dot_p * dot_a - pow(k, 2))) / dot_p;
        return true;
    }

    /**
     * Get the angles on the robot arm.
     *
     * @param[in]  p             the end point on the robot arm.
     * @param[out] joint         the joint point between the upper arm and lower arm.
     * @param[out] lowerArmAngle the lower arm angle.
     * @param[out] upperArmAngle the upper arm angle.
     * @param[out] swingAngle    the swing angle.
     * @retval     true          the robot arm can reach the point.
     * @retval     false         the robot arm can not reach the point.
     */
    static bool getRobotArmAngle(const Point3& p,
                                 Point3 *joint,
                                 double *lowerArmAngle,
                                 double *upperArmAngle,
                                 double *swingAngle) {
        Point2 xy(p.x, p.y);
        Point2 pOnRZ(xy.getLength(), p.z);

        Point2 jointOnRZ;
        if (!getJoint(pOnRZ, &jointOnRZ)) {
            return false;
        }
        Point2 xyUnit = xy.getUnit();
        *joint = Point3(xyUnit.x * jointOnRZ.x,
                        xyUnit.y * jointOnRZ.x,
                        jointOnRZ.y);
        *lowerArmAngle = jointOnRZ.getAngle();

        Point2 bOnRZ   = pOnRZ - jointOnRZ;
        *upperArmAngle = bOnRZ.getAngle();
        *swingAngle    = xy.getAngle();
        return true;
    }
};
