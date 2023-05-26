#include "calibrationmanager.h"
#include "math.h"

CalibrationManager::CalibrationManager() {}

void CalibrationManager::setCoeffs(calibrationCoeffs& coeffs) { _coeffs = coeffs; }

calibrationCoeffs CalibrationManager::getCoeffs() { return _coeffs; }

void CalibrationManager::applyCalibration(navigationPacket& pack)
{
    calibrateAccelerometer(pack);
    calibrateGyroscope(pack);
}

void CalibrationManager::calibrateAccelerometer(navigationPacket& pack)
{
    double g = 9.81;
    double aMean1 = 0.0;
    double aMean2 = 0.0;

    if (_count < _totalCount)
    {
        aMean1 += fmax(fmax(abs(pack.ax), abs(pack.ay)), abs(pack.az));
        _count++;
    }
    else if (_count - _totalCount < _totalCount)
    {
        aMean2 += fmax(fmax(abs(pack.ax), abs(pack.ay)), abs(pack.az));
        _count++;
    }
    else
    {
        aMean1 /= _totalCount;
        aMean2 /= _totalCount;
        _count = 0;

        double scaleCoeffA = 2 * g / (aMean1 + aMean2);
        double offsetA = g / scaleCoeffA - aMean1;

        if (abs(pack.ax) > abs(pack.ay))
        {
            if (abs(pack.ax) > abs(pack.az))
            {
                _coeffs.scaleCoeffA[0] = scaleCoeffA;
                _coeffs.offsetA[0] = offsetA;
            }
            else
            {
                _coeffs.scaleCoeffA[2] = scaleCoeffA;
                _coeffs.offsetA[2] = offsetA;
            }    
        }
        else
        {
            if (abs(pack.ay) > abs(pack.az))
            {
                _coeffs.scaleCoeffA[1] = scaleCoeffA;
                _coeffs.offsetA[1] = offsetA;
            }
            else
            {
                _coeffs.scaleCoeffA[2] = scaleCoeffA;
                _coeffs.offsetA[2] = offsetA;
            }
        }
    }
}

void CalibrationManager::calibrateGyroscope(navigationPacket& pack)
{
    int f = 92;
    double dt = 1.0 / f;
    double alpha = 3.1416 / 2;
    double t = 30;

    double sumG1 = 0.0;
    double sumG2 = 0.0;

    if (_count < f * t)
    {
        sumG1 += fmax(fmax(abs(pack.gx), abs(pack.gy)), abs(pack.gz));
        _count++;
    }
    else if (_count - _totalCount < _totalCount)
    {
        sumG2 += fmax(fmax(abs(pack.gx), abs(pack.gy)), abs(pack.gz));
        _count++;
    }
    else
    {
        double scaleCoeffG = 2 * alpha / ((sumG1 + sumG2) * dt);
        double offsetG = (alpha / scaleCoeffG - sumG1 * dt) / t;

        if (abs(pack.gx) > abs(pack.gy))
        {
            if (abs(pack.gx) > abs(pack.gz))
            {
                _coeffs.scaleCoeffG[0] = scaleCoeffG;
                _coeffs.offsetG[0] = offsetG;
            }
            else
            {
                _coeffs.scaleCoeffG[2] = scaleCoeffG;
                _coeffs.offsetG[2] = offsetG;
            }
        }
        else
        {
            if (abs(pack.gy) > abs(pack.gz))
            {
                _coeffs.scaleCoeffG[1] = scaleCoeffG;
                _coeffs.offsetG[1] = offsetG;
            }
            else
            {
                _coeffs.scaleCoeffG[2] = scaleCoeffG;
                _coeffs.offsetG[2] = offsetG;
            }
        }
    }
}
