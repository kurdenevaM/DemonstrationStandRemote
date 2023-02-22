#ifndef ABSTRACTADAPTER_H
#define ABSTRACTADAPTER_H

#include "architect/threaddependobject.h"
#include <QSerialPort>
#include <QVariant>
class QIODevice;
class QTimer;

class AbstractAdapter : public ThreadDependObject
{
    Q_OBJECT
public:
    explicit AbstractAdapter(QObject* parent = nullptr);

public slots:
    void onOpen(const QVariant portParams);
    void onWrite(QByteArray data);
    void onManualReconnect();

signals:
    void showInputDataExisting(bool isExist);
    void showOutputDataExisting(bool isExist);
    void sendIncomingData(QByteArray data);

protected slots:
    void onReadyRead();

protected:
    bool _isInputDataExist;
    bool _isOutputDataExist;
    QIODevice* _pPort;
    QTimer* _pDataExistingTimer;
    QVariant _lastUsedPortParams;
    int _reconnectionCounter;
    bool _isOkState;

    virtual void init() = 0;
    virtual bool open(const QVariant portParams) = 0;
    virtual void read() = 0;
    virtual void write(QByteArray data);
    virtual int startProcess(QStringList args);
    virtual int quitProcess();
    virtual bool tryToReconnectAndSaveTheDayPlease();
    virtual void thanksThisReconnectionSaveTheDay();
    void close();
    void moveToErrorState(QString errorString);
    bool getIsReadyToWork();
    bool getIsPortOpen();

private slots:
    void onDataExistingTimerOut();
};

#endif // ABSTRACTADAPTER_H
