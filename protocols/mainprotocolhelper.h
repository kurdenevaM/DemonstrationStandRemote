#ifndef MAINPROTOCOLHELPER_H
#define MAINPROTOCOLHELPER_H

#include "architect/threaddependobject.h"
#include "protocols/mainprotocol.h"
class QTimer;

class MainProtocolHelper : public ThreadDependObject
{
    Q_OBJECT
public:
    explicit MainProtocolHelper(QObject* parent = nullptr);

public slots:
    void onReceiveData(QByteArray data);

signals:
    void sendData(QByteArray data);
    void receivedPack(navigationPacket pack);
    void showCrcError();
    void showFrequency(int freq);

private slots:
    void onFreqTimerOut();

private:
    QByteArray _incomingBuffer;
    QTimer* _pFreqCountTimer;
    int _packCounter;

    virtual int startProcess(QStringList args);
    virtual int quitProcess();
};

#endif // MAINPROTOCOLHELPER_H
