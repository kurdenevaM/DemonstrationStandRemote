#include "calibrationmanager.h"

CalibrationManager::CalibrationManager() {}

void CalibrationManager::setCoefs(calibrationCoeffs& coeffs) { _coeffs = coeffs; }

void CalibrationManager::applyCalibration(navigationPacket& pack) {}
