#include <QSettings>
#include <QDebug>
#include "defines.h"
#include "SettingsStorage.h"

QString getServerUrl()
{
    QString serverUrl = SettingsStorage::getValue("General_Settings/server_url", QVariant(DEFAULT_SERVER)).toString();

    return serverUrl;
}


void setServerUrl(QString serverUrl)
{
    QSettings settings(QSettings::SystemScope,"osll","libs");
    settings.setValue("server_url",serverUrl);
}


int getServerPort()
{
    int serverPort = SettingsStorage::getValue("General_Settings/server_port", QVariant(DEFAULT_PORT)).toInt();

    return serverPort;
}


void setServerPort(int port)
{
    QSettings settings(QSettings::SystemScope,"osll","libs");
    settings.setValue("server_port",port);
}
