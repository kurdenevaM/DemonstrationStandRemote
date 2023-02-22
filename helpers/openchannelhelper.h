#ifndef OPENCHANNELHELPER_H
#define OPENCHANNELHELPER_H

#include "architect/threaddependobject.h"
#include "defines/adapterdefines.h"
class AbstractAdapter;
#include <QVariant>

class OpenChannelHelper : public ThreadDependObject
{
    Q_OBJECT
public:
    OpenChannelHelper();

public slots:
    void onOpenPort(AbstractAdapter* adapter, const QVariant settings, const QVariant standType);

signals:
    void open(const QVariant settings);
    void successOpenChannel(const QVariant standType);
    void notSuccessOpenChannel(const QString errorStr, const QVariant standType);

private:
    virtual int startProcess(QStringList args);
    virtual int quitProcess();
};

#endif // OPENCHANNELHELPER_H
