#include "openchannelhelper.h"
#include "channels/abstractadapter.h"
#include "nonlockwaiter.h"
#include <QDebug>
#include <QThread>

OpenChannelHelper::OpenChannelHelper() {}

void OpenChannelHelper::onOpenPort(AbstractAdapter* adapter, const QVariant settings, const QVariant standType)
{
    qInfo() << "OpenChannelHelper::openPort()" << adapter << settings;
    connect(this, &OpenChannelHelper::open, adapter, &AbstractAdapter::onOpen);
    bool isOk;
    int retCode = NonLockWaiter::waitReadyObject(
        adapter,
        [&]() {
            emit open(settings);
            disconnect(this, &OpenChannelHelper::open, adapter, &AbstractAdapter::onOpen);
        },
        10000, isOk);

    qInfo() << "OpenChannelHelper::openPort retCode: " << retCode;
    if (retCode == kStandardSuccessCode)
        emit successOpenChannel(standType);
    else
        emit notSuccessOpenChannel(adapter->getLastErrorString(), standType);
}

int OpenChannelHelper::startProcess(QStringList args)
{
    Q_UNUSED(args);
    return kStandardSuccessCode;
}

int OpenChannelHelper::quitProcess() { return kStandardSuccessCode; }
