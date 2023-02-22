#ifndef SERIALPORTADAPTER_H
#define SERIALPORTADAPTER_H

#include "abstractadapter.h"

class SerialPortAdapter : public AbstractAdapter
{
    Q_OBJECT
public:
    explicit SerialPortAdapter(QObject* parent = nullptr);

private:
    virtual void init();
    virtual bool open(const QVariant portParams);
    virtual void read();
};

#endif // SERIALPORTADAPTER_H
