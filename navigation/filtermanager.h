#ifndef FILTERMANAGER_H
#define FILTERMANAGER_H

#include "defines/navigationstructs.h"
#include "protocols/mainprotocol.h"

class FilterManager
{
public:
    FilterManager();

    void setSettings();
    void applyFilter(navigationPacket& pack);
};

#endif // FILTERMANAGER_H
