#ifndef USTMANAGER_H
#define USTMANAGER_H

#include "defines/navigationstructs.h"
#include "protocols/mainprotocol.h"

class UstManager
{
public:
    UstManager();
    void setCoeffs(ustCoeffs& coeffs);
    void applyUst(navigationPacket& pack);

private:
    ustCoeffs _coeffs;
};

#endif // USTMANAGER_H
