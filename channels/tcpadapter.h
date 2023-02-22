#ifndef TCPADAPTER_H
#define TCPADAPTER_H

#include "abstractadapter.h"

class TcpAdapter : public AbstractAdapter
{
public:
    explicit TcpAdapter(QObject* parent = nullptr);

private:
    virtual void init();
    virtual bool open(const QVariant portParams);
    virtual void read();
};

#endif // TCPADAPTER_H
