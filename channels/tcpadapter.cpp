#include "tcpadapter.h"
#include "defines/adapterdefines.h"

#include <QHostAddress>
#include <QTcpSocket>
#include <QThread>

TcpAdapter::TcpAdapter(QObject* parent) : AbstractAdapter(parent) {}

void TcpAdapter::init()
{
    _pPort = new QTcpSocket();
    dynamic_cast<QTcpSocket*>(_pPort)->setSocketOption(QAbstractSocket::LowDelayOption, 1);
}

bool TcpAdapter::open(const QVariant portParams)
{
    if (_pPort == nullptr)
    {
        qWarning() << "Объект порта TCP не инициализирован";
        _lastErrorString = "Объект порта TCP не инициализирован";
        return false;
    }

    EthernetSettings settings = qvariant_cast<EthernetSettings>(portParams);
    qInfo() << "try open TCP with settings: ip = " << settings.inIp << " port = " << settings.inPort << this->thread()
            << QThread::currentThread();

    dynamic_cast<QTcpSocket*>(_pPort)->connectToHost(QHostAddress(settings.inIp), settings.inPort);
    bool isOk = dynamic_cast<QTcpSocket*>(_pPort)->waitForConnected(1000);
    if (isOk == false)
    {
        qWarning() << "Не удалось подключиться по TCP";
        _lastErrorString = "Не удалось подключиться по TCP";
        return false;
    }

    qInfo() << "Sucessful connect via TCP";
    return true;
}

void TcpAdapter::read()
{
    if (_pPort != nullptr)
        emit sendIncomingData(_pPort->readAll());
}
