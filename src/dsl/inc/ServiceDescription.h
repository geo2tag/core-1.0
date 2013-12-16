#ifndef SERVICEDESCRIPTION_H
#define SERVICEDESCRIPTION_H

#include <QString>
#include <QList>
#include "Channel.h"
#include "Trackable.h"
#include "Monitor.h"
#include "MapWidgetEnum.h"
#include "TargetEnum.h"

#define CHANNEL "CHANNEL"
#define TRACKABLE "TRACKABLE"
#define MONITOR "MONITOR"
#define SERVER "server"
#define DB_NAME "database"
#define TARGET "target"
#define MAP_WIDGET "map_widget"


class ServiceDescription
{

public:
    QList<Channel>      channels;
    QList<Trackable>    trackables;
    QList<Monitor>      monitors;

    QString             server;
    QString             dbName;
    TargetEnum          target;
    MapWidgetEnum       mapWidget;

    ServiceDescription();

    const Channel *getChannelForTrackable(const Trackable& trackable) const;

    void addDescriptionEntry(const QString& prefix, const QString& mainPart);

    void checkDescriptionValidity();
};

#endif // SERVICEDESCRIPTION_H
