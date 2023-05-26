#include "navigationalgorithmmanager.h"
#include "defines/navigationstructs.h"
#include "helpers/mathhelper.h"
#include "math.h"
#include "protocols/mainprotocol.h"

NavigationAlgorithmManager::NavigationAlgorithmManager()
{
    CalibrationManager* calibrationManager = new CalibrationManager();
    _coeffs = calibrationManager->getCoeffs();
    _actualMode = NaviAlgModes::Waiting;
}

void NavigationAlgorithmManager::countActualNavigation(navigationPacket& pack)
{
    _actualSensorsData = pack;
    
    switch (_actualMode)
    {
        case NaviAlgModes::Waiting:
            if (_count < _totalCount)
            {
                _navigationPacketBuf[_count] = pack;
                _count++;
            }
            else
                _actualMode = NaviAlgModes::Alignment;
            break;

        case NaviAlgModes::Alignment:
            _actualOrientation = countInitialOrientation();
            _actualMode = NaviAlgModes::Navigation;
            break;

        case NaviAlgModes::Navigation:
            _actualOrientation = countActualOrientation();
            _actualVelosities = countActualVelosities();
            _actualCoordinates = countActualCoordinates();
            break;
    }
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

orientation NavigationAlgorithmManager::countInitialOrientation()
{
    double g = 9.81;
    double axMean = 0.0;
    double ayMean = 0.0;
    double azMean = 0.0;

    for (int i = 0; i < _totalCount; i++)
    {
        axMean += static_cast<double>(_navigationPacketBuf[i].ax);
        ayMean += static_cast<double>(_navigationPacketBuf[i].ay);
        azMean += static_cast<double>(_navigationPacketBuf[i].az);
    }

    axMean /= _totalCount;
    ayMean /= _totalCount;
    azMean /= _totalCount;

    orientation actualOrientation;
    actualOrientation.yaw = 0;
    actualOrientation.roll = atan(((azMean + _coeffs.offsetA[2]) * (-_coeffs.scaleCoeffA[2])) / ((ayMean + _coeffs.offsetA[1]) * _coeffs.scaleCoeffA[1]));
    actualOrientation.pitch = asin((axMean + _coeffs.offsetA[0]) * _coeffs.scaleCoeffA[0] / g);

    return actualOrientation;
}

orientation NavigationAlgorithmManager::countActualOrientation()
{
    int f = 92;
    double dt = 1.0 / f;
    orientation actualOrientation;

    double gx = static_cast<double>(_actualSensorsData.gx + _coeffs.offsetG[0]) * _coeffs.scaleCoeffG[0];
    double gy = static_cast<double>(_actualSensorsData.gy + _coeffs.offsetG[1]) * _coeffs.scaleCoeffG[1];
    double gz = static_cast<double>(_actualSensorsData.gz + _coeffs.offsetG[2]) * _coeffs.scaleCoeffG[2];

    actualOrientation.yaw = _actualOrientation.yaw + (gy * cos(_actualOrientation.roll) - gz * sin(_actualOrientation.roll)) / cos(_actualOrientation.pitch) * dt;
    actualOrientation.pitch = _actualOrientation.pitch + (gy * sin(_actualOrientation.roll) + gz * cos(_actualOrientation.roll)) * dt;
    actualOrientation.roll = _actualOrientation.roll + (gx - (gy * cos(_actualOrientation.roll) - gz * sin(_actualOrientation.roll)) * tan(_actualOrientation.pitch)) * dt;

    return actualOrientation;
}

double** NavigationAlgorithmManager::getOrientationMatrix()
{
    double** rollOrientationMatrix = new double* [3];
    double** pitchOrientationMatrix = new double* [3];
    double** yawOrientationMatrix = new double* [3];

    rollOrientationMatrix[0] = new double[] { 1.0, 0.0, 0.0 };
    rollOrientationMatrix[1] = new double[] { 0.0, cos(_actualOrientation.roll), sin(_actualOrientation.roll) };
    rollOrientationMatrix[2] = new double[] { 0.0, -sin(_actualOrientation.roll), cos(_actualOrientation.roll) };

    pitchOrientationMatrix[0] = new double[] { cos(_actualOrientation.pitch), sin(_actualOrientation.pitch), 0.0 };
    pitchOrientationMatrix[1] = new double[] { -sin(_actualOrientation.pitch), cos(_actualOrientation.pitch), 0.0};
    pitchOrientationMatrix[2] = new double[] { 0.0, 0.0, 1.0 };

    yawOrientationMatrix[0] = new double[] { cos(_actualOrientation.yaw), 0.0, -sin(_actualOrientation.yaw) };
    yawOrientationMatrix[1] = new double[] { 0.0, 1.0, 0.0 };
    yawOrientationMatrix[2] = new double[] { sin(_actualOrientation.yaw), 0.0, cos(_actualOrientation.yaw) };

    double** orientationMatrix = MathHelper::multiplyThreeMatrix(rollOrientationMatrix, pitchOrientationMatrix, yawOrientationMatrix);
    orientationMatrix = MathHelper::transposeMatrix(orientationMatrix);

    for (int i = 0; i < 3; i++)
    {
        delete rollOrientationMatrix[i];
        delete pitchOrientationMatrix[i];
        delete yawOrientationMatrix[i];
    }

    delete rollOrientationMatrix;
    delete pitchOrientationMatrix;
    delete yawOrientationMatrix;

    return orientationMatrix;
}

velosities NavigationAlgorithmManager::countActualVelosities()
{
    int f = 92;
    double dt = 1.0 / f;
    velosities actualVelosities;

    double ax = static_cast<double>(_actualSensorsData.ax + _coeffs.offsetA[0]) * _coeffs.scaleCoeffA[0];
    double ay = static_cast<double>(_actualSensorsData.ay + _coeffs.offsetA[1]) * _coeffs.scaleCoeffA[1];
    double az = static_cast<double>(_actualSensorsData.az + _coeffs.offsetA[2]) * _coeffs.scaleCoeffA[2];

    double dVx = ax * dt;
    double dVy = ay * dt;
    double dVz = az * dt;

    double** orientationMatrix = getOrientationMatrix();

    double dVn = orientationMatrix[0][0] * dVx + orientationMatrix[0][1] * dVy + orientationMatrix[0][2] * dVz;
    double dVUp = orientationMatrix[1][0] * dVx + orientationMatrix[1][1] * dVy + orientationMatrix[1][2] * dVz;
    double dVe = orientationMatrix[2][0] * dVx + orientationMatrix[2][1] * dVy + orientationMatrix[2][2] * dVz;

    actualVelosities.Vx = _actualVelosities.Vx + dVn;
    actualVelosities.Vy = _actualVelosities.Vy + dVUp;
    actualVelosities.Vz = _actualVelosities.Vz + dVe;

    return actualVelosities;
}

coordinates NavigationAlgorithmManager::countActualCoordinates()
{
    int f = 92;
    double dt = 1.0 / f;
    coordinates actualCoordinates;

    actualCoordinates.x = _actualCoordinates.x + _actualVelosities.Vx * dt;
    actualCoordinates.y = _actualCoordinates.y + _actualVelosities.Vy * dt;
    actualCoordinates.z = _actualCoordinates.z + _actualVelosities.Vz * dt;

    return actualCoordinates;
}
