#ifndef BLUETOOTHADAPTER_H
#define BLUETOOTHADAPTER_H

#include "abstractadapter.h"

class BluetoothAdapter : public AbstractAdapter
{
public:
    explicit BluetoothAdapter(QObject* parent = nullptr);

private:
    virtual void init();
    virtual bool open(const QVariant portParams);
    virtual void read();
};

#endif // BLUETOOTHADAPTER_H
