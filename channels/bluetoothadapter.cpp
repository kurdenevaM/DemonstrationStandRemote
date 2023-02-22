#include "bluetoothadapter.h"
#include "helpers/nonlockwaiter.h"
#include <QElapsedTimer>
#include <QtBluetooth/QBluetoothSocket>

const int kConnectWaitingTime = 15000;

BluetoothAdapter::BluetoothAdapter(QObject* parent) : AbstractAdapter(parent) {}

void BluetoothAdapter::init() { _pPort = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol); }

bool BluetoothAdapter::open(const QVariant portParams)
{
    QBluetoothServiceInfo service = qvariant_cast<QBluetoothServiceInfo>(portParams);

    dynamic_cast<QBluetoothSocket*>(_pPort)->connectToService(service);
    QElapsedTimer timer;
    timer.start();
    while (dynamic_cast<QBluetoothSocket*>(_pPort)->state() == QBluetoothSocket::ConnectingState &&
           timer.elapsed() < kConnectWaitingTime)
    {}

    return dynamic_cast<QBluetoothSocket*>(_pPort)->state() == QBluetoothSocket::ConnectedState;
}

void BluetoothAdapter::read()
{
    if (_pPort != nullptr)
        sendIncomingData(_pPort->readAll());
}
