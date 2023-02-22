#include "mainwindow.h"
#include "helpers/loghelper.h"
#include "helpers/settingshelper.h"
#include "listitemdelegate.h"
#include "ui_mainwindow.h"
#include "version/versioninfo.h"
#include <QAction>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDir>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initNavigationBar();
    initIndicatorsPixmaps();
    setupConnection();
    loadSettings();
    initShortcuts();
    ui->labelFileSize->setToolTip("Размер файла в байтах");
    ui->labelProgramVersionValue->setText(VER_FILE_VERSION_STR_INFO);
}

MainWindow::~MainWindow()
{
    ui->modellingSubwindow->~ModelingForm();
    delete ui;
}

DisplayForm* MainWindow::getDisplayForm() { return ui->receiveSubwindow; }

ModelingForm* MainWindow::getModellingForm() { return ui->modellingSubwindow; }

SettingsForm* MainWindow::getSettingsForm() { return ui->setupSubwindow; }

GraphsForm* MainWindow::getGraphsForm() { return ui->graphSubwindow; }

void MainWindow::onPortStateChanged(bool isOpen)
{
    _isMainPortOpen = isOpen;
    ui->indicatorChannel->setPixmap(isOpen ? *_pGreenIndicator : *_pGrayIndicator);
    ui->indicatorReceive->setPixmap(isOpen ? *_pRedIndicator : *_pGrayIndicator);
    ui->indicatorTransmit->setPixmap(isOpen ? *_pRedIndicator : *_pGrayIndicator);
}

void MainWindow::onShowTransmitMainPort(bool isExist)
{
    if (_isMainPortOpen == false)
        ui->indicatorTransmit->setPixmap(*_pGrayIndicator);
    else
        ui->indicatorTransmit->setPixmap(isExist ? *_pGreenIndicator : *_pRedIndicator);
}

void MainWindow::onShowReceiveMainPort(bool isExist)
{
    if (_isMainPortOpen == false)
        ui->indicatorReceive->setPixmap(*_pGrayIndicator);
    else
        ui->indicatorReceive->setPixmap(isExist ? *_pGreenIndicator : *_pRedIndicator);
}

void MainWindow::onShowFileSize(quint64 size) { ui->labelFileSize->setText(QString::number(size)); }

void MainWindow::onShowFileWritting(bool isWritting)
{
    ui->indicatorFile->setPixmap(isWritting ? *_pGreenIndicator : *_pGrayIndicator);
}

void MainWindow::onShowError(QString errorStr) { QMessageBox::critical(this, "Ошибка!", errorStr); }

void MainWindow::onShowInfo(QString infoStr) { QMessageBox::critical(this, "Информация", infoStr); }

void MainWindow::onShowFrequency(int freq) { ui->labelInputFrequency->setText(QString("%1 Гц.").arg(freq)); }

void MainWindow::initNavigationBar()
{
    _pItemPortSettings = new QListWidgetItem();
    _pItemAboutProgram = new QListWidgetItem();
    _pItemReceiving = new QListWidgetItem();
    _pItemTransmit = new QListWidgetItem();
    _pItemGraphs = new QListWidgetItem();

    _pWidgetPortSettings = new ListItemDelegate(":/NavigationIcons/settings.png", "Настройки", this);
    _pWidgetReceiving = new ListItemDelegate(":/NavigationIcons/receive.png", "Приём", this);
    _pWidgetTransmit = new ListItemDelegate(":/NavigationIcons/transmit.png", "Передача", this);
    _pWidgetAboutProgram = new ListItemDelegate(":/NavigationIcons/information.png", "Информация", this);
    _pWidgetGraphs = new ListItemDelegate(":/NavigationIcons/graphs.png", "Графики", this);

    _pItemPortSettings->setSizeHint(_pWidgetPortSettings->getItemSize());
    _pItemReceiving->setSizeHint(_pWidgetReceiving->getItemSize());
    _pItemTransmit->setSizeHint(_pWidgetTransmit->getItemSize());
    _pItemAboutProgram->setSizeHint(_pWidgetAboutProgram->getItemSize());
    _pItemGraphs->setSizeHint(_pWidgetGraphs->getItemSize());

    ui->navigationWidget->addItem(_pItemPortSettings);
    ui->navigationWidget->addItem(_pItemReceiving);
    ui->navigationWidget->addItem(_pItemTransmit);
    ui->navigationWidget->addItem(_pItemGraphs);
    ui->navigationWidget->addItem(_pItemAboutProgram);

    ui->navigationWidget->setItemWidget(_pItemPortSettings, _pWidgetPortSettings);
    ui->navigationWidget->setItemWidget(_pItemReceiving, _pWidgetReceiving);
    ui->navigationWidget->setItemWidget(_pItemTransmit, _pWidgetTransmit);
    ui->navigationWidget->setItemWidget(_pItemGraphs, _pWidgetGraphs);
    ui->navigationWidget->setItemWidget(_pItemAboutProgram, _pWidgetAboutProgram);

    ui->navigationWidget->setCurrentRow(0);
    ui->stackedWidget->setCurrentIndex(0);
    QPalette temp = this->palette();
    temp.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Base, this->palette().window().color());
    ui->navigationWidget->setPalette(temp);
}

void MainWindow::initIndicatorsPixmaps()
{
    _pGreenIndicator = new QPixmap(":/Indicators/online.png");
    _pRedIndicator = new QPixmap(":/Indicators/offline.png");
    _pGrayIndicator = new QPixmap(":/Indicators/unknow.png");
}

void MainWindow::initShortcuts()
{
    QAction* upNavigationAction = new QAction("upNavigation", this);
    QAction* downNavigationAction = new QAction("downNavigation", this);
    QAction* openLogDirAction = new QAction("openLogDir", this);

    upNavigationAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key::Key_Tab));
    downNavigationAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key::Key_Tab));
    openLogDirAction->setShortcut(tr("Ctrl+Alt+L"));

    connect(upNavigationAction, &QAction::triggered, this, &MainWindow::onUpNavigation);
    connect(downNavigationAction, &QAction::triggered, this, &MainWindow::onDownNavigation);
    connect(openLogDirAction, &QAction::triggered, this, &MainWindow::onOpenLogDir);
    this->addAction(upNavigationAction);
    this->addAction(downNavigationAction);
    this->addAction(openLogDirAction);
}

void MainWindow::setupConnection()
{
    connect(ui->navigationWidget, &QListWidget::currentRowChanged,
            [&]() { ui->stackedWidget->setCurrentIndex(ui->navigationWidget->currentRow()); });
}

void MainWindow::saveSettings()
{
    SettingsHelper::setToRegistry("height", this->height());
    SettingsHelper::setToRegistry("width", this->width());
    SettingsHelper::setToRegistry("Xcoord", this->x());
    SettingsHelper::setToRegistry("Ycoord", this->y());
}

void MainWindow::loadSettings()
{
    int x = SettingsHelper::getFromRegistry("Xcoord", -100000).toInt();
    int y = SettingsHelper::getFromRegistry("Ycoord", -100000).toInt();
    if (x != -100000 && y != -100000)
        move(x, y);

    int w = SettingsHelper::getFromRegistry("width", -10000).toInt();
    int h = SettingsHelper::getFromRegistry("height", -10000).toInt();
    if (w != -10000 && h != -10000)
        resize(w, h);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    ui->modellingSubwindow->~ModelingForm();
    ui->setupSubwindow->~SettingsForm();
    saveSettings();
    event->accept();
}

//===== Shortcuts =============================================
void MainWindow::onDownNavigation()
{
    if (ui->navigationWidget->currentRow() > 0)
        ui->navigationWidget->setCurrentRow(ui->navigationWidget->currentRow() - 1);
    else
        ui->navigationWidget->setCurrentRow(ui->navigationWidget->count() - 1);
}

void MainWindow::onUpNavigation()
{
    if (ui->navigationWidget->currentRow() < ui->navigationWidget->count() - 1)
        ui->navigationWidget->setCurrentRow(ui->navigationWidget->currentRow() + 1);
    else
        ui->navigationWidget->setCurrentRow(0);
}

void MainWindow::onOpenLogDir() { QDesktopServices::openUrl(QUrl(getLogPath())); }
