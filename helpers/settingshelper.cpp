#include "settingshelper.h"

#include "defines/adapterdefines.h"
#include <QApplication>
#include <QDebug>
#include <QJsonDocument>
#include <QSettings>

SettingsHelper::~SettingsHelper() {}

QVariant SettingsHelper::getFromRegistry(QString key, QVariant defaultValue)
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    if (settings.contains(key))
        return settings.value(key);
    else
        return defaultValue;
}

void SettingsHelper::setToRegistry(QString key, QVariant value)
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    settings.setValue(key, value);
}

SettingsHelper::SettingsHelper() {}
