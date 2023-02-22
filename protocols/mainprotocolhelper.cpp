#include "mainprotocolhelper.h"
#include "helpers/crchelper.h"
#include <QDebug>
#include <QTimer>

MainProtocolHelper::MainProtocolHelper(QObject* parent) : ThreadDependObject(parent) { _packCounter = 0; }

void MainProtocolHelper::onReceiveData(QByteArray data)
{
    _incomingBuffer.append(data);

    while (_incomingBuffer.contains(kStandardSync))
    {
        _incomingBuffer.remove(0, _incomingBuffer.indexOf(kStandardSync));
        quint8 len = _incomingBuffer.at(2);
        if (_incomingBuffer.length() < len)
            return;

        quint16 crc = CrcHelper::countCrc16(reinterpret_cast<unsigned char*>(_incomingBuffer.data()), len - 2);
        quint16 receivedCrc;
        memcpy(&receivedCrc, _incomingBuffer.data() + len - 2, 2);
        if (crc != receivedCrc)
        {
            _incomingBuffer.remove(0, 1);
            emit showCrcError();
            continue;
        }

        quint8 id = _incomingBuffer.at(3);
        if (id == NavigationPack)
        {
            navigationPacket pack;
            memcpy(&pack, _incomingBuffer.data(), len);
            _packCounter++;
            emit receivedPack(pack);
            _incomingBuffer.remove(0, len);
        }
        else
            _incomingBuffer.remove(0, 1);
    }
}

void MainProtocolHelper::onFreqTimerOut()
{
    emit showFrequency(_packCounter);
    _packCounter = 0;
}

int MainProtocolHelper::startProcess(QStringList args)
{
    Q_UNUSED(args);
    _pFreqCountTimer = new QTimer();
    _pFreqCountTimer->setTimerType(Qt::PreciseTimer);
    _pFreqCountTimer->setInterval(1000);
    connect(_pFreqCountTimer, &QTimer::timeout, this, &MainProtocolHelper::onFreqTimerOut);
    _pFreqCountTimer->start();
    return kStandardSuccessCode;
}

int MainProtocolHelper::quitProcess() { return kStandardSuccessCode; }
