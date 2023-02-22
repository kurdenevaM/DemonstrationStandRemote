#include "udpadapter.h"
#include "defines/adapterdefines.h"
#include <QUdpSocket>

UdpAdapter::UdpAdapter(QObject* parent) : AbstractAdapter(parent) {}

void UdpAdapter::init()
{
    _pPort = new QUdpSocket();
    dynamic_cast<QUdpSocket*>(_pPort)->setSocketOption(QAbstractSocket::LowDelayOption, 1);
}

bool UdpAdapter::open(const QVariant portParams)
{
    if (_pPort == nullptr)
    {
        qWarning() << "Объекта порта UDP не инициализирован";
        _lastErrorString = "Объекта порта UDP не инициализирован";
        return false;
    }

    EthernetSettings settings = qvariant_cast<EthernetSettings>(portParams);
    qInfo() << "try open UDP with settings: ip = " << settings.outIp << " port = " << settings.outPort;
    bool isOk = dynamic_cast<QUdpSocket*>(_pPort)->bind(QHostAddress(settings.outIp), settings.outPort);
    if (isOk == false)
    {
        qWarning() << "Не удалось подключиться по UDP";
        _lastErrorString = "Не удалось подключиться по UDP";
        return false;
    }

    qInfo() << "Sucessful connect via UDP";
    return true;
}

void UdpAdapter::read()
{
    if (_pPort == nullptr)
        return;

    QByteArray datagram;
    int sizeDatagram = dynamic_cast<QUdpSocket*>(_pPort)->pendingDatagramSize();

    datagram.resize(sizeDatagram);
    dynamic_cast<QUdpSocket*>(_pPort)->readDatagram(datagram.data(), datagram.size());

    emit sendIncomingData(datagram);
}
