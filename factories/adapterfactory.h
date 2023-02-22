#ifndef ADAPTERFACTORY_H
#define ADAPTERFACTORY_H

#include "defines/adapterdefines.h"
#include <QObject>

class AbstractAdapter;

class AdapterFactory : public QObject
{
    Q_OBJECT
public:
    explicit AdapterFactory(QObject* parent = nullptr);
    ~AdapterFactory();
    AbstractAdapter* getAdapter(QVariant adapterType);
    void deleteAdapter(AbstractAdapter* adapter);

signals:
    void deInit();

private:
    struct AdapterDescriptor
    {
        PortAdapterType type;
        AbstractAdapter* object;
        QThread* thread;
    };

    QList<AdapterDescriptor> _productsList;
};

#endif // ADAPTERFACTORY_H
