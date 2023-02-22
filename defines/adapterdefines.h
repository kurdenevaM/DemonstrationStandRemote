#ifndef ADAPTERDEFINES_H
#define ADAPTERDEFINES_H

#include <QObject>
#include <QSerialPort>

//Ключи json для настройки соединения: type (Eth, Com...), tcpIp, tcpPort, comPort, isNeedToConnect
const QString kPortKey = "port";
const QString kIpKey = "ip";
const QString kSlaveKey = "slave";
const QString kConnectionTypeKey = "type";
const QString kComPortNameKey = "comPort";
const QString kDeviceTypeKey = "deviceType";
const QString kNeedConnectKey = "needConnect";
const QString kTrueValue = "true";
const QString kFalseValue = "false";

//todo -> надо бы перейти на разделение по протоколам TCP/UDP...
const QStringList kConnectionTcpTypeAvailableNames = {"Ethernet (TCP)", "TCP"};
const QStringList kConnectionUdpTypeAvailableNames = {"Ethernet (UDP)", "UDP"};
const QStringList kConnectionModbusTypeAvailableNames = {"Ethernet (Modbus)", "Modbus"};
const QStringList kConnectionSerialTypeAvailableNames = {"Последовательный порт", "Serial port", "COM-порт", "COM-port",
                                                         "COM"};
const QStringList kConnectionBluetoothTypeAvailableNames = {"BT", "Bluetooth"};
const QString kDeviceTypeTable = "table";
const QString kDeviceTypeCamera = "camera";

/**
 * @brief The AdapterType enum - тип адаптера с которым работаем
 */
enum class PortAdapterType
{
    AdapterErrorType = -1,
    AdapterSerialPort,
    AdapterTcpPort,
    AdapterBluetooth,
};
Q_DECLARE_METATYPE(PortAdapterType);

enum class ManagerType
{
    ManagerErrorType = -1,
    RotationTableManager,
    TermoCameraManager,
};
Q_DECLARE_METATYPE(ManagerType);

enum class StandType
{
    StandErrorType = -1,
    AcutronicX3Type,
    AcutronicX2Type,
    AcutronicX1Type,
    AcuitasX2Type,
    SomeAutoCameraType,
};
Q_DECLARE_METATYPE(StandType);

enum class ChannelType
{
    tlmt,
    cons,
    rotary,
    snsOutput,
    snsInput
};
Q_DECLARE_METATYPE(ChannelType);

enum class EthernetConnectionType
{
    Tcp,
    Udp,
};
Q_DECLARE_METATYPE(EthernetConnectionType);

enum class PortStatus
{
    Open = 0,
    Closed = 1,
    Error = 2
};
Q_DECLARE_METATYPE(PortStatus);

typedef struct ComSettings
{
    QString portName;
    int baudrate;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    QSerialPort::DataBits dataBits;
} ComSettings;
Q_DECLARE_METATYPE(ComSettings);

typedef struct EthernetSettings
{
    QString inIp;
    QString outIp;
    unsigned short inPort;
    unsigned short outPort;
} EthernetSettings;
Q_DECLARE_METATYPE(EthernetSettings);

typedef struct ModbusSettings
{
    EthernetSettings tcpSettings;
    int slaveNum;
} ModbusSettings;
Q_DECLARE_METATYPE(ModbusSettings);

typedef struct ModbusUniversalMsg
{
    qint64 regAddress;
    qint64 value;
    bool isReadOperation;
} ModbusUniversalMsg;
Q_DECLARE_METATYPE(ModbusUniversalMsg);

#endif // ADAPTERDEFINES_H
