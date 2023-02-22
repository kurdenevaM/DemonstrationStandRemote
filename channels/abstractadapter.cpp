#include "abstractadapter.h"

#include "helpers/nonlockwaiter.h"
#include <QDebug>
#include <QIODevice>
#include <QTimer>
#include <QVariant>

const int kMaxReconnectionRetries = 10;

AbstractAdapter::AbstractAdapter(QObject* parent) : ThreadDependObject(parent)
{
    _pPort = nullptr;
    _pDataExistingTimer = nullptr;
    _reconnectionCounter = 0;
    _isOkState = true;
    _isInputDataExist = false;
    _isOutputDataExist = false;
}

int AbstractAdapter::startProcess(QStringList args)
{
    Q_UNUSED(args);

    _pDataExistingTimer = new QTimer();
    _pDataExistingTimer->setInterval(500);
    _pDataExistingTimer->setSingleShot(false);
    connect(_pDataExistingTimer, &QTimer::timeout, this, &AbstractAdapter::onDataExistingTimerOut);
    _isInputDataExist = false;
    _isOutputDataExist = false;

    init();
    if (_pPort != nullptr)
    {
        connect(_pPort, &QIODevice::readyRead, this, &AbstractAdapter::onReadyRead);
        return 0;
    }
    else
    {
        _lastErrorString = "Не удалось создать объект порта";
        return 1;
    }
}

int AbstractAdapter::quitProcess()
{
    close();
    if (_pDataExistingTimer != nullptr)
    {
        _pDataExistingTimer->stop();
        delete _pDataExistingTimer;
        _pDataExistingTimer = nullptr;
    }
    if (_pPort != nullptr)
    {
        delete _pPort;
        _pPort = nullptr;
    }

    return 0;
}

bool AbstractAdapter::tryToReconnectAndSaveTheDayPlease()
{
    _reconnectionCounter++;
    if (_reconnectionCounter < kMaxReconnectionRetries)
    {
        NonLockWaiter::wait(2); //Подождём немного, потом снова попробуем
        close();
        bool isOk = open(_lastUsedPortParams);
        if (isOk)
            return true;
        else
            return tryToReconnectAndSaveTheDayPlease();
    }
    else
    {
        _lastErrorString =
            QString(
                "Возникла ошибка: %1. Для её устранения было проведено %2 попыток переподключения, но это не помогло")
                .arg(_lastErrorString)
                .arg(kMaxReconnectionRetries);
        return false;
    }
}

void AbstractAdapter::thanksThisReconnectionSaveTheDay() { _reconnectionCounter = 0; }

void AbstractAdapter::onOpen(const QVariant portParams)
{
    _reconnectionCounter = 0;
    bool isOk = open(portParams);
    _lastUsedPortParams = portParams;

    if (isOk == false) // Пытаемся спасти ситуацию
    {
        isOk = tryToReconnectAndSaveTheDayPlease();
        if (isOk == true)
            thanksThisReconnectionSaveTheDay(); //Не забываем благодарить!
    }

    if (isOk)
        _pDataExistingTimer->start();
    emit ready(isOk ? kStandardSuccessCode : kStandardUnsuccessCode);
}

void AbstractAdapter::onWrite(QByteArray data)
{
    _isOutputDataExist = true;
    if (getIsReadyToWork())
        write(data);
}

void AbstractAdapter::onManualReconnect()
{
    _reconnectionCounter = 0; //Сбрасываем счётчик перед очередной попыткой
    bool isOk = tryToReconnectAndSaveTheDayPlease();
    if (isOk == true)
    {
        thanksThisReconnectionSaveTheDay();
        emit showInfo("Подключение установлено!");
    }
    else
    {
        emit moveToErrorState("Не удалось установить соединение...");
    }
}

void AbstractAdapter::write(QByteArray data)
{
    int writtenBytes = _pPort->write(data);
    if (writtenBytes < data.length())
    {
        _lastErrorString = "Не удалось записать все данные в канал связи";
        bool isOk = tryToReconnectAndSaveTheDayPlease();
        if (isOk == true)
            write(data);
        else
            moveToErrorState("Не удалось установить соединение...");
    }
}

void AbstractAdapter::onReadyRead()
{
    _isInputDataExist = true;
    read(); // Тут всё надёжно - у нас уже есть сигнал readyRead - значит с портом порядок
}

void AbstractAdapter::close()
{
    if (getIsPortOpen())
        _pPort->close();
}

void AbstractAdapter::moveToErrorState(QString errorString)
{
    _isOkState = false;
    emit showError(errorString);
}

bool AbstractAdapter::getIsReadyToWork() { return (getIsPortOpen() && _isOkState); }

bool AbstractAdapter::getIsPortOpen() { return (_pPort != nullptr && _pPort->isOpen()); }

void AbstractAdapter::onDataExistingTimerOut()
{
    emit showInputDataExisting(_isInputDataExist);
    emit showOutputDataExisting(_isOutputDataExist);
    _isInputDataExist = false;
    _isOutputDataExist = false;
}
