#include "mathhelper.h"

const double kRadToDeg = 57.295779513082320876;
const double kDegToRad = 1 / kRadToDeg;

MathHelper::MathHelper() {}

float MathHelper::convertTo180Angle(float angle)
{
    while (angle > 360.0f) angle -= 360.0f;
    while (angle < 0.0f) angle += 360.0f;

    return angle;
}

double MathHelper::convertTo180Angle(double angle)
{
    while (angle > 360.0f) angle -= 360.0f;
    while (angle < 0.0f) angle += 360.0f;

    return angle;
}

void MathHelper::correctToFrom0To360Angle(float& angle)
{
    if (angle > 360.0f)
        angle -= 360.0f;

    if (angle < 0.0f)
        angle += 360.0f;
}

void MathHelper::correctToFrom0To360Angle(double& angle)
{
    if (angle > 360.0f)
        angle -= 360.0f;

    if (angle < 0.0f)
        angle += 360.0f;
}

double MathHelper::fromRadToDeg(double angle) { return angle * kRadToDeg; }

double MathHelper::fromDegToRad(double angle) { return angle * kDegToRad; }
