#ifndef MAINPROTOCOL_H
#define MAINPROTOCOL_H
#include <QMetaType>
#include <QtGlobal>

//const short kStandardSync = 0xdada;
const QByteArray kStandardSync = "\xda\xda";
const unsigned short kStandardSyncInNumber = 0xdada;
enum Identificators
{
    NavigationPack = 0x01,
    LedsPack = 0x02,
};
enum Leds
{
    LD3 = 9,
    LD4 = 8,
    LD5 = 10,
    LD6 = 15,
    LD7 = 11,
    LD8 = 14,
    LD9 = 12,
    LD10 = 13
};

enum LedsModes
{
    Off = 0,
    ShowUp = 1,
    ShowDown = 2,
    ShowLevel = 3,
    ShowNorth = 4,
    Test = 5,
};

#pragma pack(push, 1)
typedef struct header
{
    unsigned short sync;
    unsigned char len;
    unsigned char id;
    unsigned short counter;
} header;

typedef struct navigationPacket
{
    header header;
    short gx;
    short gy;
    short gz;
    short ax;
    short ay;
    short az;
    short mx;
    short my;
    short mz;
    short t;
    unsigned short crc;
} navigationPacket;

typedef struct LedsPacket
{
    header header;
    union
    {
        unsigned char led;
        struct
        {
            unsigned char ld3mode : 1;
            unsigned char ld4mode : 1;
            unsigned char ld5mode : 1;
            unsigned char ld6mode : 1;
            unsigned char ld7mode : 1;
            unsigned char ld8mode : 1;
            unsigned char ld9mode : 1;
            unsigned char ld10mode : 1;
        };
    };
    char mode;
    unsigned short crc;
} LedsPacket;

#pragma pack(pop)

#endif // MAINPROTOCOL_H
