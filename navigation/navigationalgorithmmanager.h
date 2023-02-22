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
};

#endif // NAVIGATIONALGORITHMMANAGER_H
