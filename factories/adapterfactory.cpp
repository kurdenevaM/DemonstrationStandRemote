#include "adapterfactory.h"
#include "channels/bluetoothadapter.h"
#include "channels/serialportadapter.h"
#include "channels/tcpadapter.h"
#include "channels/udpadapter.h"

#include "helpers/nonlockwaiter.h"
#include <QDebug>
#include <QEventLoop>
#include <QThread>

AdapterFactory::AdapterFactory(QObject* parent) : QObject(parent) { qInfo() << "AdapterFactory created"; }

AdapterFactory::~AdapterFactory() { qInfo() << "AdapterFactory destroyed"; }

AbstractAdapter* AdapterFactory::getAdapter(QVariant adapterType)
{
    PortAdapterType curType;
    if (kConnectionSerialTypeAvailableNames.contains(adapterType.toString(), Qt::CaseInsensitive))
        curType = PortAdapterType::AdapterSerialPort;
    else if (kConnectionTcpTypeAvailableNames.contains(adapterType.toString(), Qt::CaseInsensitive))
        curType = PortAdapterType::AdapterTcpPort;
    else if (kConnectionBluetoothTypeAvailableNames.contains(adapterType.toString(), Qt::CaseInsensitive))
        curType = PortAdapterType::AdapterBluetooth;
    else
        curType = PortAdapterType::AdapterErrorType;

    AbstractAdapter* retObject = nullptr;
    switch (curType)
    {
        case PortAdapterType::AdapterSerialPort:
            retObject = new SerialPortAdapter();
            break;
        case PortAdapterType::AdapterTcpPort:
            retObject = new TcpAdapter();
            break;
        case PortAdapterType::AdapterBluetooth:
            retObject = new BluetoothAdapter();
            break;
        default:
            return nullptr;
    }

    QThread* adapterThread = new QThread();
    retObject->moveToThread(adapterThread);
    connect(adapterThread, &QThread::started, retObject, &ThreadDependObject::onStartProcessWithoutArgs);

    bool isReady;
    int retCode = NonLockWaiter::waitReadyObject(
        retObject, [&]() { adapterThread->start(); }, 1000, isReady);

    if (isReady == false || retCode != 0)
    {
        delete retObject;
        delete adapterThread;
        return nullptr;
    }

    AdapterDescriptor descriptor;
    descriptor.object = retObject;
    descriptor.type = curType;
    descriptor.thread = adapterThread;
    _productsList.append(descriptor);

    return retObject;
}

void AdapterFactory::deleteAdapter(AbstractAdapter* adapter)
{
    qInfo() << "deleteAdapter";
    int index = -1;
    for (int i = 0; i < _productsList.count(); i++)
        if (_productsList.at(i).object == adapter)
        {
            index = i;
            break;
        }

    if (index == -1) // Да идите вы нафиг с такими шутками
        return;      // Товар не наш и перерабатывать его мы не будем!

    QEventLoop* loop = new QEventLoop();
    AdapterDescriptor deletedProduct = _productsList.takeAt(index);
    connect(this, &AdapterFactory::deInit, deletedProduct.object, &ThreadDependObject::onQuitProcess);
    connect(deletedProduct.object, &ThreadDependObject::ready, loop, &QEventLoop::exit);
    emit deInit();
    loop->exec();

    deletedProduct.thread->terminate();

    delete loop;
    delete deletedProduct.thread;
    delete deletedProduct.object;
}
