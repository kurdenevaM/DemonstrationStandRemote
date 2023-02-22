#ifndef SETTINGSHELPER_H
#define SETTINGSHELPER_H

#include "defines/registrykeys.h"
#include <QSerialPort>
#include <QVariant>

class SettingsHelper
{
public:
    SettingsHelper();
    ~SettingsHelper();

    static QVariant getFromRegistry(QString key, QVariant defaultValue);
    static void setToRegistry(QString key, QVariant value);
};

#endif // SETTINGSHELPER_H
