#include "modelingform.h"
#include "helpers/crchelper.h"
#include "helpers/settingshelper.h"
#include "protocols/mainprotocol.h"
#include "ui_modelingform.h"
#include <QDebug>

const QString kSwitchOnLabel = "Включить выдачу";
const QString kSwitchOffLabel = "Выключить выдачу";

ModelingForm::ModelingForm(QWidget* parent) : QWidget(parent), ui(new Ui::ModelingForm)
{
    ui->setupUi(this);
    setupConnections();
    ui->groupBoxLeds->setVisible(false);
}

ModelingForm::~ModelingForm()
{
    saveSettings();
    delete ui;
}

void ModelingForm::onLedsModeInfoChanged()
{
    if (ui->comboBoxLesModes->currentIndex() == LedsModes::Test)
        ui->groupBoxLeds->setVisible(true);
    else
        ui->groupBoxLeds->setVisible(false);
    sendLedsMode();
}

void ModelingForm::sendLedsMode()
{
    qDebug() << "sendLedsMode";
    LedsPacket pack;
    pack.header.counter = 0;
    pack.header.sync = kStandardSyncInNumber;
    pack.header.id = LedsPack;
    pack.header.len = sizeof(pack);

    pack.mode = ui->comboBoxLesModes->currentIndex();
    pack.ld3mode = ui->pushButtonLd3->isChecked() ? 1 : 0;
    pack.ld4mode = ui->pushButtonLd4->isChecked() ? 1 : 0;
    pack.ld5mode = ui->pushButtonLd5->isChecked() ? 1 : 0;
    pack.ld6mode = ui->pushButtonLd6->isChecked() ? 1 : 0;
    pack.ld7mode = ui->pushButtonLd7->isChecked() ? 1 : 0;
    pack.ld8mode = ui->pushButtonLd8->isChecked() ? 1 : 0;
    pack.ld9mode = ui->pushButtonLd9->isChecked() ? 1 : 0;
    pack.ld10mode = ui->pushButtonLd10->isChecked() ? 1 : 0;

    pack.crc = CrcHelper::countCrc16(reinterpret_cast<unsigned char*>(&pack), sizeof(pack) - 2);
    emit sendedLedsMode(QByteArray(reinterpret_cast<char*>(&pack), sizeof(pack)));
}

void ModelingForm::setupConnections()
{
    connect(ui->comboBoxLesModes, &QComboBox::currentTextChanged, this, &ModelingForm::onLedsModeInfoChanged);
    connect(ui->pushButtonLd3, &QPushButton::clicked, this, &ModelingForm::onLedsModeInfoChanged);
    connect(ui->pushButtonLd4, &QPushButton::clicked, this, &ModelingForm::onLedsModeInfoChanged);
    connect(ui->pushButtonLd5, &QPushButton::clicked, this, &ModelingForm::onLedsModeInfoChanged);
    connect(ui->pushButtonLd6, &QPushButton::clicked, this, &ModelingForm::onLedsModeInfoChanged);
    connect(ui->pushButtonLd7, &QPushButton::clicked, this, &ModelingForm::onLedsModeInfoChanged);
    connect(ui->pushButtonLd8, &QPushButton::clicked, this, &ModelingForm::onLedsModeInfoChanged);
    connect(ui->pushButtonLd9, &QPushButton::clicked, this, &ModelingForm::onLedsModeInfoChanged);
    connect(ui->pushButtonLd10, &QPushButton::clicked, this, &ModelingForm::onLedsModeInfoChanged);
}

void ModelingForm::loadSettings() {}

void ModelingForm::saveSettings() {}
