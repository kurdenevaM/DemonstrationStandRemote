#include "displayform.h"
#include "ui_displayform.h"
#include <QDateTime>
#include <QTimer>

DisplayForm::DisplayForm(QWidget* parent) : QWidget(parent), ui(new Ui::DisplayForm)
{
    ui->setupUi(this);
    initObjects();
    initConnections();
    _pUpdateActualityTimer->start();
    _isWaitFirstPack = true;
    _lastActualityState = true;
    _thereIsPack = false;
    _errorCounter = 0;
    onUpdateDataActuality();
}

DisplayForm::~DisplayForm() { delete ui; }

void DisplayForm::onUpdateForm(navigationPacket pack)
{
    _thereIsPack = true;
    onUpdateDataActuality();
    if (_isWaitFirstPack)
    {
        _errorCounter = -1;
        onCrcError();
        _isWaitFirstPack = false;
    }
    ui->labelPackNumber->setText(QString("Счётчик пакетов: %1").arg(pack.header.counter));
    showSensorsData(pack);
}

void DisplayForm::onShowOrientation(orientation value)
{
    ui->widgetYpr->setYPR(value.yaw, value.pitch, value.roll);
    ui->widgetYpr2->setYPR(value.yaw, value.pitch, value.roll);
    ui->lineEditYaw->setText(QString::number(value.yaw));
    ui->lineEditPitch->setText(QString::number(value.pitch));
    ui->lineEditRoll->setText(QString::number(value.roll));

    ui->lineEditYaw_2->setText(QString::number(value.yaw));
    ui->lineEditPitch_2->setText(QString::number(value.pitch));
    ui->lineEditRoll_2->setText(QString::number(value.roll));
}

void DisplayForm::onCrcError()
{
    _errorCounter++;
    ui->labelCrcErrors->setText(QString("Ошибок CRC: %1").arg(_errorCounter));
}

void DisplayForm::onUpdateDataActuality()
{
    if (_thereIsPack == _lastActualityState)
        return;

    _lastActualityState = _thereIsPack;
    _thereIsPack = false;

    if (_thereIsPack == false)
    {

        _isWaitFirstPack = true;
    }

    ui->lineEditAx->setEnabled(_thereIsPack);
    ui->lineEditAy->setEnabled(_thereIsPack);
    ui->lineEditAz->setEnabled(_thereIsPack);
    ui->lineEditWx->setEnabled(_thereIsPack);
    ui->lineEditWy->setEnabled(_thereIsPack);
    ui->lineEditWz->setEnabled(_thereIsPack);

    ui->lineEditHeight_2->setEnabled(_thereIsPack);
    ui->lineEditLatitude_2->setEnabled(_thereIsPack);
    ui->lineEditLongitude_2->setEnabled(_thereIsPack);

    ui->lineEditPitch->setEnabled(_thereIsPack);
    ui->lineEditPitch_2->setEnabled(_thereIsPack);
    ui->lineEditYaw->setEnabled(_thereIsPack);
    ui->lineEditYaw_2->setEnabled(_thereIsPack);
    ui->lineEditRoll->setEnabled(_thereIsPack);
    ui->lineEditRoll_2->setEnabled(_thereIsPack);

    ui->lineEditVe->setEnabled(_thereIsPack);
    ui->lineEditVn->setEnabled(_thereIsPack);
    ui->lineEditVup->setEnabled(_thereIsPack);
}

void DisplayForm::initObjects()
{
    ui->widgetYpr->setBackgroundColor(this->palette().window().color());
    ui->widgetYpr2->setBackgroundColor(this->palette().window().color());
    _pUpdateActualityTimer = new QTimer();

    _pUpdateActualityTimer->setTimerType(Qt::CoarseTimer);
    _pUpdateActualityTimer->setInterval(3000);
    _pUpdateActualityTimer->setSingleShot(false);

    _pGreenIndicator = new QPixmap(":/Indicators/online.png");
    _pRedIndicator = new QPixmap(":/Indicators/offline.png");
    _pGrayIndicator = new QPixmap(":/Indicators/unknow.png");
}

void DisplayForm::initConnections()
{
    connect(ui->widgetYpr, &YPRPainter::switchedYprMode, [&]() { ui->stackedWidget->setCurrentIndex(1); });
    connect(ui->widgetYpr2, &YPRPainter::switchedYprMode, [&]() { ui->stackedWidget->setCurrentIndex(0); });

    connect(_pUpdateActualityTimer, &QTimer::timeout, this, &DisplayForm::onUpdateDataActuality);
}

void DisplayForm::showSensorsData(const navigationPacket& data)
{
    ui->lineEditAx->setText(QString::number(data.ax));
    ui->lineEditAy->setText(QString::number(data.ay));
    ui->lineEditAz->setText(QString::number(data.az));
    ui->lineEditWx->setText(QString::number(data.gx));
    ui->lineEditWy->setText(QString::number(data.gy));
    ui->lineEditWz->setText(QString::number(data.gz));
}
