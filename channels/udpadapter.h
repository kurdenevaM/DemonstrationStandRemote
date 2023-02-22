#ifndef UDPADAPTER_H
#define UDPADAPTER_H

#include "abstractadapter.h"

class UdpAdapter : public AbstractAdapter
{
public:
    explicit UdpAdapter(QObject* parent = nullptr);

private:
    virtual void init();
    virtual bool open(const QVariant portParams);
    virtual void read();
};

#endif // UDPADAPTER_H
