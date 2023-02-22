#ifndef NAVIGATIONMANAGER_H
#define NAVIGATIONMANAGER_H

#include "architect/threaddependobject.h"
#include "defines/navigationstructs.h"
#include "protocols/mainprotocol.h"
class FilterManager;
class CalibrationManager;
class UstManager;
class NavigationAlgorithmManager;
class QTimer;

class NavigationManager : public ThreadDependObject
{
    Q_OBJECT
public:
    explicit NavigationManager(QObject* parent = nullptr);

public slots:
    void onReceiveSensorsData(navigationPacket pack);

signals:
    void showActualOrientation(orientation value);
    void showActualCoordinates(coordinates value);
    void showActualVelosities(velosities value);
    void showSensorsData(navigationPacket value);
private slots:
    void onUpdateView();

private:
    FilterManager* _pFilterManager;
    CalibrationManager* _pCalibrationManager;
    UstManager* _pUstManager;
    NavigationAlgorithmManager* _pAlgoManager;

    QTimer* _pUpdateViewTimer;

    virtual int startProcess(QStringList args);
    virtual int quitProcess();
};

#endif // NAVIGATIONMANAGER_H
