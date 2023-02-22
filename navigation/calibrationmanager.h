#ifndef CALIBRATIONMANAGER_H
#define CALIBRATIONMANAGER_H

#include "defines/navigationstructs.h"
#include "protocols/mainprotocol.h"

class CalibrationManager
{
public:
    CalibrationManager();
    void setCoefs(calibrationCoeffs& coeffs);
    void applyCalibration(navigationPacket& pack);

private:
    calibrationCoeffs _coeffs;
};

#endif // CALIBRATIONMANAGER_H
