#ifndef NAVIGATIONALGORITHMMANAGER_H
#define NAVIGATIONALGORITHMMANAGER_H

#include "defines/navigationstructs.h"
#include "protocols/mainprotocol.h"

class NavigationAlgorithmManager
{
public:
    NavigationAlgorithmManager();
    void countActualNavigation(navigationPacket& pack);
    void setActualMode(NaviAlgModes mode);
    orientation getOrientation();
    coordinates getCoordinates();
    velosities getVelosities();
    orientation getOrientationInDeg();
    navigationPacket getActualSensorsData();

private:
    NaviAlgModes _actualMode;
    orientation _actualOrientation;
    velosities _actualVelosities;
    coordinates _actualCoordinates;
    navigationPacket _actualSensorsData;
    calibrationCoeffs _coeffs;

    int _count = 0;
    int _totalCount = 5000;
    navigationPacket _navigationPacketBuf[5000];

    orientation countInitialOrientation();
    orientation countActualOrientation();
    double** getOrientationMatrix();
    velosities countActualVelosities();
    coordinates countActualCoordinates();
};

#endif // NAVIGATIONALGORITHMMANAGER_H
