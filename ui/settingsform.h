#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QSerialPort>
#include <QVariant>
#include <QWidget>
class QBluetoothServiceDiscoveryAgent;
class QBluetoothDeviceDiscoveryAgent;
class QListWidgetItem;
#include <QBluetoothDeviceInfo>
#include <QBluetoothServiceInfo>

namespace Ui
{
class SettingsForm;
}

class SettingsForm : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsForm(QWidget* parent = nullptr);
    ~SettingsForm();

public slots:
    void onPortStateChanged(bool isOpen);

signals:
    void openPort(QVariant type, QVariant settings);
    void closePort();
    void startFileWrite(QString fName);
    void stopFileWrite();

private slots:
    void onOpenClosePort();
    void onUpdatePorts();
    void onStartStopFileWrite();
    void onChooseDir();
    void onFileNameChanged();

    void onStartBluetoothDiscovery();
    void onServiceDiscovered(const QBluetoothServiceInfo& serviceInfo);
    void onDeviceDiscovered(const QBluetoothDeviceInfo& info);
    void onDiscoveryFinished();

private:
    Ui::SettingsForm* ui;
    QList<QWidget*> _portSettingsWidgets;
    QStringList _curPortsList;
    QPalette _goodPalette;
    QPalette _errorPalette;

    QBluetoothDeviceDiscoveryAgent* _pBluetoothDiscoveryAgent;
    QMap<QListWidgetItem*, QBluetoothServiceInfo> _discoveredServices;
    QMap<QListWidgetItem*, QBluetoothDeviceInfo> _discoveredDevices;

    void initVariables();
    void initConnections();
    void loadSettings();
    void saveSettings();

    void setEnabledAllPortFields(bool isEnabled);

    int getBaudRate(QString str);
    QSerialPort::StopBits getStopBits(QString str);
    QSerialPort::DataBits getDataBits(QString str);
    QSerialPort::Parity getParity(QString str);

    void checkStateFileButton();
    bool getIsGoodFileName();
    void setupDiscoveryAgent();
};

#endif // SETTINGSFORM_H
