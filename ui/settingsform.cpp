#include "settingsform.h"
#include "defines/adapterdefines.h"
#include "helpers/settingshelper.h"
#include "ui_settingsform.h"
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QBluetoothLocalDevice>
#include <QBluetoothServiceDiscoveryAgent>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QSerialPortInfo>

const QString kOpenLabel = "Открыть";
const QString kCloseLabel = "Закрыть";
const QString kStartFileWrite = "Начать запись";
const QString kStopFileWrite = "Закончить запись";

SettingsForm::SettingsForm(QWidget* parent) : QWidget(parent), ui(new Ui::SettingsForm)
{
    ui->setupUi(this);
    initVariables();
    initConnections();
    onUpdatePorts();
    loadSettings();
    onFileNameChanged();
    checkStateFileButton();
    ui->pushButtonStartStopFileWrite->setText(kStartFileWrite);
    _goodPalette = this->palette();
    _errorPalette = _goodPalette;
    _errorPalette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Base, QColor(255, 10, 10));

    setupDiscoveryAgent();
    _pBluetoothDiscoveryAgent->start();
}

SettingsForm::~SettingsForm()
{
    saveSettings();
    delete ui;
}

void SettingsForm::onPortStateChanged(bool isOpen)
{
    setEnabledAllPortFields(!isOpen);
    ui->pushOpenPort->setEnabled(true);
    ui->pushOpenPort->setText(isOpen ? kCloseLabel : kOpenLabel);
    if (ui->labelBluetoothStatus->text() == "Выполняется подключение...")
        ui->labelBluetoothStatus->setText(isOpen ? "Подключено" : "Не подключено");
}

//=========== Private slots ======================================
void SettingsForm::onOpenClosePort()
{
    setEnabledAllPortFields(false);
    if (ui->pushOpenPort->text() == kOpenLabel)
    {
        if (ui->comboBox->currentIndex() == 0)
        {
            ComSettings settings;
            settings.portName = ui->comboPortName->currentText();
            settings.baudrate = getBaudRate(ui->comboSpeed->currentText());
            settings.dataBits = getDataBits(ui->comboWordLen->currentText());
            settings.parity = getParity(ui->comboParity->currentText());
            settings.stopBits = getStopBits(ui->comboStopBit->currentText());
            QVariant container;
            container.setValue(settings);
            emit openPort("COM", container);
        }
        else
        {
            if (_pBluetoothDiscoveryAgent != nullptr && _pBluetoothDiscoveryAgent->isActive())
                _pBluetoothDiscoveryAgent->stop();

            ui->labelBluetoothStatus->setText("Выполняется подключение...");
            QVariant container;
            container.setValue(_discoveredDevices.value(ui->bluetoothDevices->currentItem()));
            emit openPort("BT", container);
        }
    }
    else
        emit closePort();

    saveSettings();
}

void SettingsForm::onUpdatePorts()
{
    _curPortsList.clear();

    QList<QSerialPortInfo> info = QSerialPortInfo::availablePorts();
    for (int i = info.count() - 1; i >= 0; --i) _curPortsList.append(info.at(i).portName());

    if (ui->comboPortName->isEnabled())
    {
        QString curPortName = ui->comboPortName->currentText();
        ui->comboPortName->clear();
        ui->comboPortName->addItems(_curPortsList);

        for (int i = 0; i < ui->comboPortName->count(); ++i)
            if (ui->comboPortName->itemText(i) == curPortName)
            {
                ui->comboPortName->setCurrentIndex(i);
                break;
            }
    }
}

void SettingsForm::onStartStopFileWrite()
{
    if (ui->pushButtonStartStopFileWrite->text() == kStartFileWrite)
    {
        emit startFileWrite(ui->lineEditDir->text() + "/" + ui->lineEditName->text() + ".txt");
        ui->pushButtonStartStopFileWrite->setText(kStopFileWrite);
    }
    else
    {
        emit stopFileWrite();
        ui->pushButtonStartStopFileWrite->setText(kStartFileWrite);
        checkStateFileButton();
    }
}

void SettingsForm::onChooseDir()
{
    QString path = SettingsHelper::getFromRegistry("path", "").toString();
    path = QFileDialog::getExistingDirectory(this, "Выберите каталог для хранения файла телеметрии", path,
                                             QFileDialog::ShowDirsOnly);

    if (path.isEmpty() == false)
    {
        ui->lineEditDir->setText(path);
        SettingsHelper::setToRegistry("path", path);
    }
    checkStateFileButton();
}

void SettingsForm::onFileNameChanged() { checkStateFileButton(); }

void SettingsForm::onStartBluetoothDiscovery()
{
    if (_pBluetoothDiscoveryAgent != nullptr)
    {
        if (_pBluetoothDiscoveryAgent->isActive())
            _pBluetoothDiscoveryAgent->stop();
        delete _pBluetoothDiscoveryAgent;
    }

    ui->bluetoothDevices->clear();
    _discoveredServices.clear();
    setupDiscoveryAgent();
    ui->labelBluetoothStatus->setText("Поиск устройств...");
    _pBluetoothDiscoveryAgent->start();
}

void SettingsForm::onServiceDiscovered(const QBluetoothServiceInfo& serviceInfo)
{
    QMapIterator<QListWidgetItem*, QBluetoothServiceInfo> i(_discoveredServices);
    while (i.hasNext())
    {
        i.next();
        if (serviceInfo.device().address() == i.value().device().address())
            return;
    }

    QString remoteName;
    if (serviceInfo.device().name().isEmpty())
        remoteName = serviceInfo.device().address().toString();
    else
        remoteName = serviceInfo.device().name();

    QListWidgetItem* item =
        new QListWidgetItem(QString::fromLatin1("%1 %2").arg(remoteName, serviceInfo.serviceName()));

    _discoveredServices.insert(item, serviceInfo);
    ui->bluetoothDevices->addItem(item);
}

void SettingsForm::onDeviceDiscovered(const QBluetoothDeviceInfo& info)
{
    QString label = QString("%1 %2").arg(info.address().toString()).arg(info.name());
    QList<QListWidgetItem*> items = ui->bluetoothDevices->findItems(label, Qt::MatchExactly);
    if (items.empty())
    {
        QListWidgetItem* item = new QListWidgetItem(label);
        //        QBluetoothLocalDevice::Pairing pairingStatus = localDevice->pairingStatus(info.address());
        //        if (pairingStatus == QBluetoothLocalDevice::Paired || pairingStatus == QBluetoothLocalDevice::AuthorizedPaired)
        //            item->setTextColor(QColor(Qt::green));
        //        else
        //            item->setTextColor(QColor(Qt::black));

        _discoveredDevices.insert(item, info);
        ui->bluetoothDevices->addItem(item);
    }
}

void SettingsForm::onDiscoveryFinished()
{
    ui->labelBluetoothStatus->setText("Поиск завершен");
    delete _pBluetoothDiscoveryAgent;
    _pBluetoothDiscoveryAgent = nullptr;
}

//============ Private ==============================================
void SettingsForm::initVariables()
{
    _portSettingsWidgets << ui->comboParity << ui->comboPortName << ui->comboSpeed << ui->comboStopBit
                         << ui->comboWordLen << ui->pushOpenPort;
}

void SettingsForm::initConnections()
{
    connect(ui->pushOpenPort, &QPushButton::clicked, this, &SettingsForm::onOpenClosePort);
    connect(ui->pushRefresh, &QPushButton::clicked, this, &SettingsForm::onUpdatePorts);

    connect(ui->lineEditName, &QLineEdit::textChanged, this, &SettingsForm::onFileNameChanged);
    connect(ui->pushButtonChooseDir, &QPushButton::clicked, this, &SettingsForm::onChooseDir);
    connect(ui->pushButtonStartStopFileWrite, &QPushButton::clicked, this, &SettingsForm::onStartStopFileWrite);

    connect(ui->pushButtonStartBluetoothDiscovery, &QPushButton::clicked, this,
            &SettingsForm::onStartBluetoothDiscovery);
}

void SettingsForm::loadSettings()
{

    ui->lineEditDir->setText(SettingsHelper::getFromRegistry("path", "").toString());
    ui->comboBox->setCurrentIndex(SettingsHelper::getFromRegistry("channelType", 0).toInt());
    ui->stackedWidget->setCurrentIndex(ui->comboBox->currentIndex());

    ui->comboParity->setCurrentIndex(SettingsHelper::getFromRegistry("parity", 0).toInt());
    ui->comboSpeed->setCurrentIndex(SettingsHelper::getFromRegistry("speed", 6).toInt());
    ui->comboWordLen->setCurrentIndex(SettingsHelper::getFromRegistry("dataLen", 3).toInt());
    ui->comboStopBit->setCurrentIndex(SettingsHelper::getFromRegistry("stopBit", 0).toInt());

    QString portName = SettingsHelper::getFromRegistry("portName", "").toString();
    QString portNameSns = SettingsHelper::getFromRegistry("portNameSns", "").toString();

    onUpdatePorts();

    for (int i = 0; i < ui->comboPortName->count(); ++i)
        if (ui->comboPortName->itemText(i) == portName)
        {
            ui->comboPortName->setCurrentIndex(i);
            break;
        }
}

void SettingsForm::saveSettings()
{
    SettingsHelper::setToRegistry("channelType", ui->comboBox->currentIndex());
    SettingsHelper::setToRegistry("parity", ui->comboParity->currentIndex());
    SettingsHelper::setToRegistry("speed", ui->comboSpeed->currentIndex());
    SettingsHelper::setToRegistry("dataLen", ui->comboWordLen->currentIndex());
    SettingsHelper::setToRegistry("stopBit", ui->comboStopBit->currentIndex());
    SettingsHelper::setToRegistry("portName", ui->comboPortName->currentText());
    SettingsHelper::setToRegistry("path", ui->lineEditDir->text());
}

void SettingsForm::setEnabledAllPortFields(bool isEnabled)
{
    foreach (QWidget* widget, _portSettingsWidgets)
        widget->setEnabled(isEnabled);
}

int SettingsForm::getBaudRate(QString str) { return str.toInt(); }

QSerialPort::StopBits SettingsForm::getStopBits(QString str)
{
    if (str == "1")
        return QSerialPort::OneStop;
    else if (str == "1,5")
        return QSerialPort::OneAndHalfStop;
    else if (str == "2")
        return QSerialPort::TwoStop;
    else
        return QSerialPort::UnknownStopBits;
}

QSerialPort::DataBits SettingsForm::getDataBits(QString str)
{
    if (str == "5")
        return QSerialPort::Data5;
    else if (str == "6")
        return QSerialPort::Data6;
    else if (str == "7")
        return QSerialPort::Data7;
    else if (str == "8")
        return QSerialPort::Data8;
    else
        return QSerialPort::UnknownDataBits;
}

QSerialPort::Parity SettingsForm::getParity(QString str)
{
    if (str == "Нет")
        return QSerialPort::NoParity;
    else if (str == "Чет")
        return QSerialPort::EvenParity;
    else if (str == "Нечет")
        return QSerialPort::OddParity;
    else if (str == "Маркер (1)")
        return QSerialPort::MarkParity;
    else if (str == "Пробел (0)")
        return QSerialPort::SpaceParity;
    else
        return QSerialPort::UnknownParity;
}

void SettingsForm::checkStateFileButton()
{
    bool isGoodFileName = getIsGoodFileName();
    if (ui->lineEditDir->text().isEmpty() == false && isGoodFileName)
        ui->pushButtonStartStopFileWrite->setEnabled(true);
    else
        ui->pushButtonStartStopFileWrite->setEnabled(false);
}

bool SettingsForm::getIsGoodFileName()
{
    ui->lineEditName->setToolTip("");
    if (ui->lineEditDir->text().isEmpty())
    {
        ui->lineEditName->setPalette(_goodPalette);
        return true;
    }
    if (ui->lineEditName->text().isEmpty())
    {
        ui->lineEditName->setPalette(_goodPalette);
        return false;
    }
    QDir workDir(ui->lineEditDir->text());
    QStringList list = workDir.entryList(QDir::Filter::Files);
    QString myFileName = ui->lineEditName->text() + ".txt";
    bool isGoodName = !list.contains(myFileName);
    ui->lineEditName->setPalette(isGoodName ? _goodPalette : _errorPalette);
    ui->lineEditName->setToolTip("Файл с таким именем уже есть в этом каталоге");
    return isGoodName;
}

void SettingsForm::setupDiscoveryAgent()
{
    _pBluetoothDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    connect(_pBluetoothDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this,
            &SettingsForm::onDeviceDiscovered);
    connect(_pBluetoothDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this,
            &SettingsForm::onDiscoveryFinished);
    connect(_pBluetoothDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled, this,
            &SettingsForm::onDiscoveryFinished);

    _pBluetoothDiscoveryAgent->setInquiryType(QBluetoothDeviceDiscoveryAgent::GeneralUnlimitedInquiry);
}
