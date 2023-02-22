#ifndef CRCHELPER_H
#define CRCHELPER_H

class CrcHelper
{
public:
    CrcHelper();
    static unsigned short countCrc16(unsigned char* pcBlock, unsigned short len);
};

#endif // CRCHELPER_H
