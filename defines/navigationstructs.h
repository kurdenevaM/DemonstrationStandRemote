#ifndef NAVIGATIONSTRUCTS_H
#define NAVIGATIONSTRUCTS_H

enum class NaviAlgModes
{
    Waiting,
    Alignment,
    Navigation,
};

typedef struct calibrationCoeffs
{
    short offsetA[3];
    short offsetG[3];
    short offsetM[3];
    double scaleCoeffA[3];
    double scaleCoeffG[3];
    double scaleCoeffM[3];
} calibrationCoeffs;

typedef struct ustCoeffs
{
    double uYaw;
    double uPitch;
    double uRoll;
} ustCoeffs;

typedef struct orientation
{
    double yaw;
    double pitch;
    double roll;
} orientation;

typedef struct coordinates
{
    double x;
    double y;
    double z;
} coordinates;

typedef struct velosities
{
    double Vx;
    double Vy;
    double Vz;
} velosities;

#endif // NAVIGATIONSTRUCTS_H
