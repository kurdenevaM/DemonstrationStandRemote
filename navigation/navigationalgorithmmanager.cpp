#include "navigationalgorithmmanager.h"
#include "defines/navigationstructs.h"
#include "helpers/mathhelper.h"
#include "math.h"
#include "protocols/mainprotocol.h"

NavigationAlgorithmManager::NavigationAlgorithmManager() {}

void NavigationAlgorithmManager::countActualNavigation(navigationPacket& pack)
{
    _actualSensorsData = pack;
    _actualOrientation.yaw = 0;
    _actualOrientation.roll = asin(static_cast<double>(pack.ay) / 18000.0);
    _actualOrientation.pitch = asin(static_cast<double>(pack.ax) / 18000.0);
}

void NavigationAlgorithmManager::setActualMode(NaviAlgModes mode) { _actualMode = mode; }

orientation NavigationAlgorithmManager::getOrientation() { return _actualOrientation; }

coordinates NavigationAlgorithmManager::getCoordinates() { return _actualCoordinates; }

velosities NavigationAlgorithmManager::getVelosities() { return _actualVelosities; }

orientation NavigationAlgorithmManager::getOrientationInDeg()
{
    orientation temp;
    temp.yaw = MathHelper::fromRadToDeg(_actualOrientation.yaw);
    temp.pitch = MathHelper::fromRadToDeg(_actualOrientation.pitch);
    temp.roll = MathHelper::fromRadToDeg(_actualOrientation.roll);
    return temp;
}

navigationPacket NavigationAlgorithmManager::getActualSensorsData() { return _actualSensorsData; }
