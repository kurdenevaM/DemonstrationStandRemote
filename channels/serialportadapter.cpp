#include "serialportadapter.h"
#include "defines/adapterdefines.h"

#include <QDebug>
#include <QVariant>

SerialPortAdapter::SerialPortAdapter(QObject* parent) : AbstractAdapter(parent) {}

void SerialPortAdapter::init() { _pPort = new QSerialPort(); }

bool SerialPortAdapter::open(const QVariant portParams)
{
    if (_pPort == nullptr)
    {
        qWarning() << "Объект последовательного порта не инициализирован";
        _lastErrorString = "Объект последовательного порта не инициализирован";
        return false;
    }

    ComSettings settings = qvariant_cast<ComSettings>(portParams);
    dynamic_cast<QSerialPort*>(_pPort)->setPortName(settings.portName);
    qInfo() << "Try connect via serial port with settings: name " << settings.portName << " baud: " << settings.baudrate
            << "parity: " << settings.parity << " stop bits: " << settings.stopBits
            << "data bits: " << settings.dataBits;

    bool isOk = dynamic_cast<QSerialPort*>(_pPort)->open(QIODevice::ReadWrite);
    if (isOk == false)
    {
        qWarning() << "Can't open serial port " << settings.portName;
        _lastErrorString = QString("Не удалось открыть последовательный порт %1").arg(settings.portName);
        return false;
    }

    isOk &= dynamic_cast<QSerialPort*>(_pPort)->setBaudRate(settings.baudrate);
    isOk &= dynamic_cast<QSerialPort*>(_pPort)->setParity(settings.parity);
    isOk &= dynamic_cast<QSerialPort*>(_pPort)->setStopBits(settings.stopBits);
    isOk &= dynamic_cast<QSerialPort*>(_pPort)->setFlowControl(QSerialPort::NoFlowControl);

    if (isOk == false)
    {
        qWarning() << "Can't set settings for port " << settings.portName;
        _lastErrorString = QString("Не удалось задать настройки последовательного порта %1").arg(settings.portName);
        return false;
    }

    qInfo() << "Sucessful connect via SerialPort (" << settings.portName << ")";
    return true;
}

void SerialPortAdapter::read()
{
    if (_pPort != nullptr)
        emit sendIncomingData(_pPort->readAll());
}
