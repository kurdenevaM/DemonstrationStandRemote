#ifndef REGISTRYKEYS_H
#define REGISTRYKEYS_H

#include <QMap>
#include <QString>

const QString kRegKeyStandType = "standType";

const QString kRegKeyDeviceTypeTable = "table";
const QString kRegKeyDeviceTypeCamera = "camera";
const QString kRegKeyConnectionType = "ConnectType";
const QString kRegKeyCorrection1 = "correction1";
const QString kRegKeyCorrection2 = "correction2";
const QString kRegKeyCorrection3 = "correction3";
const QString kRegKeyTcpIp = "tcpIp";
const QString kRegKeyTcpPort = "tcpPort";
const QString kRegKeyUdpIp = "udpIp";
const QString kRegKeyUdpPort = "udpPort";
const QString kRegKeyModbusIp = "modbusIp";
const QString kRegKeyModbusPort = "modbusPort";
const QString kRegKeySerialPort = "SerialPort";

const QString kRegDirAcutronicX3 = "AcutronicX3/";
const QString kRegDirAcutronicX2 = "AcutronicX2/";
const QString kRegDirAcutronicX1 = "AcutronicX1/";
const QString kRegDirAcuitasX2 = "AcuitasX2/";
const QString kRegDirSomeAutoTermocamera = "SomeAutoTermocamera/";

const QMap<QString, QString> kDefaultValuesMap =
    QMap<QString, QString>({{kRegDirAcutronicX3 + kRegKeyDeviceTypeTable + kRegKeyTcpIp, "192.168.53.1"},
                            {kRegDirAcutronicX3 + kRegKeyDeviceTypeTable + kRegKeyTcpPort, "9878"},
                            {kRegDirAcutronicX3 + kRegKeyDeviceTypeTable + kRegKeySerialPort, "COM1"},
                            {kRegDirAcutronicX3 + kRegKeyCorrection1, "0"},

                            {kRegDirAcutronicX2 + kRegKeyDeviceTypeTable + kRegKeyTcpIp, "192.168.53.1"},
                            {kRegDirAcutronicX2 + kRegKeyDeviceTypeTable + kRegKeyTcpPort, "9878"},
                            {kRegDirAcutronicX2 + kRegKeyDeviceTypeTable + kRegKeySerialPort, "COM1"},
                            {kRegDirAcutronicX2 + kRegKeyCorrection1, "0"},
                            {kRegDirAcutronicX2 + kRegKeyDeviceTypeCamera + kRegKeyTcpIp, "192.168.40.40"},
                            {kRegDirAcutronicX2 + kRegKeyDeviceTypeCamera + kRegKeyTcpPort, "502"},
                            {kRegDirAcutronicX2 + kRegKeyDeviceTypeCamera + kRegKeySerialPort, "COM1"},

                            {kRegDirAcutronicX1 + kRegKeyDeviceTypeTable + kRegKeyTcpIp, "192.168.53.1"},
                            {kRegDirAcutronicX1 + kRegKeyDeviceTypeTable + kRegKeyTcpPort, "9878"},
                            {kRegDirAcutronicX1 + kRegKeyDeviceTypeTable + kRegKeySerialPort, "COM1"},
                            {kRegDirAcutronicX1 + kRegKeyCorrection1, "0"},

                            {kRegDirAcuitasX2 + kRegKeyDeviceTypeTable + kRegKeyTcpIp, "192.168.53.1"},
                            {kRegDirAcuitasX2 + kRegKeyDeviceTypeTable + kRegKeyTcpPort, "9878"},
                            {kRegDirAcuitasX2 + kRegKeyDeviceTypeTable + kRegKeySerialPort, "COM1"},
                            {kRegDirAcuitasX2 + kRegKeyCorrection1, "0"},
                            {kRegDirAcuitasX2 + kRegKeyDeviceTypeCamera + kRegKeyTcpIp, "192.168.40.40"},
                            {kRegDirAcuitasX2 + kRegKeyDeviceTypeCamera + kRegKeyTcpPort, "502"},
                            {kRegDirAcuitasX2 + kRegKeyDeviceTypeCamera + kRegKeySerialPort, "COM1"},

                            {kRegDirSomeAutoTermocamera + kRegKeyDeviceTypeCamera + kRegKeyTcpIp, "192.168.40.40"},
                            {kRegDirSomeAutoTermocamera + kRegKeyDeviceTypeCamera + kRegKeyTcpPort, "502"},
                            {kRegDirSomeAutoTermocamera + kRegKeyDeviceTypeCamera + kRegKeySerialPort, "COM1"}});

#endif // REGISTRYKEYS_H
