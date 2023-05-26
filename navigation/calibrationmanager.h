#ifndef CALIBRATIONMANAGER_H
#define CALIBRATIONMANAGER_H

#include "defines/navigationstructs.h"
#include "protocols/mainprotocol.h"

class CalibrationManager
{
public:
    CalibrationManager();
    void setCoeffs(calibrationCoeffs& coeffs);
    calibrationCoeffs getCoeffs();
    void applyCalibration(navigationPacket& pack);

private:
    calibrationCoeffs _coeffs;
    int _totalCount = 5000;
    int _count;
    navigationPacket _navigationPacketBuf[5000];

    void calibrateAccelerometer(navigationPacket& pack);
    void calibrateGyroscope(navigationPacket& pack);
};

#endif // CALIBRATIONMANAGER_H
