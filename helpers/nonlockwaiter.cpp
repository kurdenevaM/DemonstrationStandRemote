#include "nonlockwaiter.h"
#include <QElapsedTimer>
#include <QEventLoop>
#include <QTimer>

NonLockWaiter::NonLockWaiter(QObject* parent) : QObject(parent) {}

void NonLockWaiter::wait(int msecs)
{
    QEventLoop* loop = new QEventLoop();
    QTimer* timer = new QTimer();
    timer->setSingleShot(true);
    QObject::connect(timer, &QTimer::timeout, loop, &QEventLoop::quit);
    timer->start(msecs);
    loop->exec();

    timer->stop();
    delete timer;
    delete loop;
}

int NonLockWaiter::waitReadyObject(ThreadDependObject* object, std::function<void()> signalEmitFunc, int maxWaitingTime,
                                   bool& isReady, int waitingQuantum)
{
    int code = kStandardUnsuccessCode;
    bool isTimerUp = false;
    bool isObjectReady = false;
    QTimer* maxWaitingTimeController = new QTimer();
    maxWaitingTimeController->setSingleShot(true);

    connect(maxWaitingTimeController, &QTimer::timeout, [&isTimerUp]() { isTimerUp = true; });
    QMetaObject::Connection tempConnection = connect(object, &ThreadDependObject::ready, [&](int retCode) {
        isObjectReady = true;
        code = retCode;
    });

    maxWaitingTimeController->start(maxWaitingTime);
    signalEmitFunc();
    while (isTimerUp == false && isObjectReady == false) wait(waitingQuantum);

    disconnect(tempConnection);
    maxWaitingTimeController->stop();
    delete maxWaitingTimeController;

    isReady = isObjectReady;
    return code;
}
