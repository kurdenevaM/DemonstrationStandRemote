#include "navigationmanager.h"
#include "calibrationmanager.h"
#include "filtermanager.h"
#include "navigationalgorithmmanager.h"
#include "ustmanager.h"
#include <QDebug>
#include <QTimer>

NavigationManager::NavigationManager(QObject* parent) : ThreadDependObject(parent) {}

void NavigationManager::onReceiveSensorsData(navigationPacket pack)
{
    _pCalibrationManager->applyCalibration(pack);
    _pUstManager->applyUst(pack);
    _pFilterManager->applyFilter(pack);
    _pAlgoManager->countActualNavigation(pack);
}

void NavigationManager::onUpdateView()
{
    emit showActualOrientation(_pAlgoManager->getOrientationInDeg());
    emit showActualVelosities(_pAlgoManager->getVelosities());
    emit showActualCoordinates(_pAlgoManager->getCoordinates());
    emit showSensorsData(_pAlgoManager->getActualSensorsData());
}

int NavigationManager::startProcess(QStringList args)
{
    Q_UNUSED(args);
    _pUstManager = new UstManager();
    _pFilterManager = new FilterManager();
    _pCalibrationManager = new CalibrationManager();
    _pAlgoManager = new NavigationAlgorithmManager();
    _pUpdateViewTimer = new QTimer();
    _pUpdateViewTimer->setInterval(200);
    _pUpdateViewTimer->setTimerType(Qt::CoarseTimer);
    connect(_pUpdateViewTimer, &QTimer::timeout, this, &NavigationManager::onUpdateView);
    _pUpdateViewTimer->start();
    return kStandardSuccessCode;
}

int NavigationManager::quitProcess() { return kStandardSuccessCode; }
