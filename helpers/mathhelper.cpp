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

double** MathHelper::multiplyMatrix(double** mat1, double** mat2)
{
    int n = 3;
    double** result = new double* [n];

    for (int i = 0; i < n; i++)
        result[i] = new double[n];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            result[i][j] = 0;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                result[i][j] += mat1[i][k] * mat2[k][j];

    return result;
}

double** MathHelper::multiplyThreeMatrix(double** mat1, double** mat2, double** mat3)
{
    return multiplyMatrix(multiplyMatrix(mat1, mat2), mat3);
}

double** MathHelper::transposeMatrix(double** mat)
{
    int n = 3;
    double** transposedMat = new double* [n];

    for (int i = 0; i < n; i++)
        transposedMat[i] = new double[n];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            transposedMat[i][j] = mat[j][i];

    return transposedMat;
}
