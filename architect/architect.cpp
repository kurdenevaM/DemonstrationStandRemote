#include "architect.h"
#include "channels/abstractadapter.h"
#include "defines/adapterdefines.h"
#include "defines/navigationstructs.h"
#include "factories/adapterfactory.h"
#include "helpers/filewriterhelper.h"
#include "helpers/loghelper.h"
#include "helpers/nonlockwaiter.h"
#include "helpers/openchannelhelper.h"
#include "navigation/navigationmanager.h"
#include "protocols/mainprotocolhelper.h"
#include "ui/displayform.h"
#include "ui/graphsform.h"
#include "ui/mainwindow.h"
#include "ui/modelingform.h"
#include "ui/settingsform.h"
#include "version/versioninfo.h"
#include <QApplication>
#include <QDebug>
#include <QThread>

Architect::Architect(QObject* parent, int argc, char* argv[]) : QObject(parent)
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setStyle("Fusion");
    _pApplication = new QApplication(argc, argv);
    registrateApplication();
    registerMetaTypes();
    //    setupLog();
    initialisation();
    _pMainWindow->getModellingForm()->loadSettings();
    _pMainWindow->show();
}

int Architect::startApplication() { return _pApplication->exec(); }

void Architect::initialisation()
{
    initObjects();
    initConnections();
}

void Architect::initObjects()
{
    qInfo() << "Architect::initObjects()";
    _pFactory = new AdapterFactory();
    _pMainWindow = new MainWindow();
    _pChannelAdapter = nullptr;

    _pOpenHelper = new OpenChannelHelper();
    _pOpenHelperThread = new QThread();
    _pOpenHelper->moveToThread(_pOpenHelperThread);
    connect(_pOpenHelperThread, &QThread::started, _pOpenHelper, &OpenChannelHelper::onStartProcessWithoutArgs);
    bool isReady;
    NonLockWaiter::waitReadyObject(
        _pOpenHelper, [&]() { _pOpenHelperThread->start(); }, 1000, isReady);

    _pFileHelper = new FileWriterHelper();
    _pFileWriteThread = new QThread();
    _pFileHelper->moveToThread(_pFileWriteThread);
    connect(_pFileWriteThread, &QThread::started, _pFileHelper, &OpenChannelHelper::onStartProcessWithoutArgs);
    NonLockWaiter::waitReadyObject(
        _pFileHelper, [&]() { _pFileWriteThread->start(); }, 1000, isReady);

    _pNaviManager = new NavigationManager();
    _pNaviManagerThread = new QThread();
    _pNaviManager->moveToThread(_pNaviManagerThread);
    connect(_pNaviManagerThread, &QThread::started, _pNaviManager, &NavigationManager::onStartProcessWithoutArgs);
    NonLockWaiter::waitReadyObject(
        _pNaviManager, [&]() { _pNaviManagerThread->start(); }, 1000, isReady);

    _pMainProtocolHelper = new MainProtocolHelper();
    _pMainProtocolThread = new QThread();
    _pMainProtocolHelper->moveToThread(_pMainProtocolThread);
    connect(_pMainProtocolThread, &QThread::started, _pMainProtocolHelper,
            &OpenChannelHelper::onStartProcessWithoutArgs);
    NonLockWaiter::waitReadyObject(
        _pMainProtocolHelper, [&]() { _pMainProtocolThread->start(); }, 1000, isReady);
}

void Architect::initConnections()
{
    qInfo() << "Architect::initConnections()";
    connect(_pMainProtocolHelper, &MainProtocolHelper::receivedPack, _pNaviManager,
            &NavigationManager::onReceiveSensorsData);
    connect(_pMainProtocolHelper, &MainProtocolHelper::receivedPack, _pMainWindow->getGraphsForm(),
            &GraphsForm::onReceiveSensorsData);
    connect(_pMainProtocolHelper, &MainProtocolHelper::showFrequency, _pMainWindow, &MainWindow::onShowFrequency);

    //====== OpenHelper ======================================
    connect(this, &Architect::openPort, _pOpenHelper, &OpenChannelHelper::onOpenPort);
    connect(_pOpenHelper, &OpenChannelHelper::successOpenChannel, this, &Architect::onSuccessOpenChannel);
    connect(_pOpenHelper, &OpenChannelHelper::notSuccessOpenChannel, this, &Architect::onNotSuccessOpenChannel);

    //====== MainWindow ======================================
    connect(this, &Architect::showError, _pMainWindow, &MainWindow::onShowError);
    connect(this, &Architect::portStateChanged, _pMainWindow, &MainWindow::onPortStateChanged);

    //======= SettingsForm ===================================
    connect(this, &Architect::portStateChanged, _pMainWindow->getSettingsForm(), &SettingsForm::onPortStateChanged);
    connect(_pMainWindow->getSettingsForm(), &SettingsForm::openPort, this, &Architect::onConnectToMainChannel);
    connect(_pMainWindow->getSettingsForm(), &SettingsForm::closePort, this, &Architect::onDisconnectToMainChannel);

    //======== FileWriterHelper ==============================
    connect(_pFileHelper, &FileWriterHelper::updateFileState, _pMainWindow, &MainWindow::onShowFileWritting);
    connect(_pFileHelper, &FileWriterHelper::updateWrittingSize, _pMainWindow, &MainWindow::onShowFileSize);
    connect(_pMainWindow->getSettingsForm(), &SettingsForm::startFileWrite, _pFileHelper,
            &FileWriterHelper::onStartWrite);
    connect(_pMainWindow->getSettingsForm(), &SettingsForm::stopFileWrite, _pFileHelper,
            &FileWriterHelper::onStopWrite);

    //======= DisplayForm ====================================
    connect(_pNaviManager, &NavigationManager::showSensorsData, _pMainWindow->getDisplayForm(),
            &DisplayForm::onUpdateForm);
    connect(_pMainProtocolHelper, &MainProtocolHelper::showCrcError, _pMainWindow->getDisplayForm(),
            &DisplayForm::onCrcError);
    connect(_pNaviManager, &NavigationManager::showActualOrientation, _pMainWindow->getDisplayForm(),
            &DisplayForm::onShowOrientation);
}

void Architect::onCloseApplication()
{
    qInfo() << "Architect::onCloseApplication()";
    foreach (QMetaObject::Connection connection, _connections)
        disconnect(connection);

    _pFactory->deleteAdapter(_pChannelAdapter);

    _pMainWindow->close();
}

void Architect::onConnectToMainChannel(QVariant type, QVariant settings)
{
    qDebug() << "connect to mainChannel" << type << settings;
    _pChannelAdapter = _pFactory->getAdapter(type);
    qDebug() << _pChannelAdapter;
    emit openPort(_pChannelAdapter, settings, "Main");
}

void Architect::onDisconnectToMainChannel()
{
    _pFactory->deleteAdapter(_pChannelAdapter);
    emit portStateChanged(false);
}

void Architect::onSuccessOpenChannel(QVariant standType)
{
    Q_UNUSED(standType);
    connect(_pChannelAdapter, &AbstractAdapter::sendIncomingData, _pMainProtocolHelper,
            &MainProtocolHelper::onReceiveData);
    connect(_pMainProtocolHelper, &MainProtocolHelper::sendData, _pChannelAdapter, &AbstractAdapter::onWrite);
    //    connect(_pChannelAdapter, &AbstractAdapter::sendIncomingData, _pFileHelper, &FileWriterHelper::onWrite);
    connect(_pMainProtocolHelper, &MainProtocolHelper::receivedPack, _pFileHelper,
            &FileWriterHelper::onWriteNavigationPack);
    connect(_pChannelAdapter, &AbstractAdapter::showInputDataExisting, _pMainWindow,
            &MainWindow::onShowReceiveMainPort);
    connect(_pChannelAdapter, &AbstractAdapter::showOutputDataExisting, _pMainWindow,
            &MainWindow::onShowTransmitMainPort);
    connect(_pChannelAdapter, &AbstractAdapter::showError, _pMainWindow, &MainWindow::onShowError);
    connect(_pChannelAdapter, &AbstractAdapter::showInfo, _pMainWindow, &MainWindow::onShowInfo);

    connect(_pMainWindow->getModellingForm(), &ModelingForm::sendedLedsMode, _pChannelAdapter,
            &AbstractAdapter::onWrite);

    emit portStateChanged(true);
}

void Architect::onNotSuccessOpenChannel(QString errorStr, QVariant standType)
{
    Q_UNUSED(standType);
    emit showError(QString("Ошибка открытия основного порта: %1").arg(errorStr));
    _pFactory->deleteAdapter(_pChannelAdapter);
    emit portStateChanged(false);
}

void Architect::registrateApplication()
{
    QCoreApplication::setApplicationVersion(VER_FILE_VERSION_STR);
    QCoreApplication::setOrganizationName(VER_COMPANY_NAME);
    QCoreApplication::setApplicationName(VER_PRODUCTNAME_STR);
}

void Architect::registerMetaTypes()
{
    qRegisterMetaType<PortAdapterType>("PortAdapterType");
    qRegisterMetaType<navigationPacket>("navigationPacket");
    qRegisterMetaType<orientation>("orientation");
    qRegisterMetaType<velosities>("velosities");
    qRegisterMetaType<coordinates>("coordinates");
    qRegisterMetaType<ustCoeffs>("ustCoeffs");
    qRegisterMetaType<calibrationCoeffs>("calibrationCoeffs");
    qRegisterMetaType<NaviAlgModes>("NaviAlgModes");

    qRegisterMetaType<Leds>("Leds");
    qRegisterMetaType<LedsModes>("LedsModes");
}
