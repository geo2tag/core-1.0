#ifndef TRACKABLE_H
#define TRACKABLE_H

#include <QString>
#include "IsOnlineEnum.h"
#include "MapObject.h"

#define CHANNEL_NAME_PLACEHOLDER "CHANNEL_NAME_PLACEHOLDER"
#define IS_ONLINE_PLACEHOLDER "IS_ONLINE_PLACEHOLDER"

class Trackable: public MapObject
{

public:
    QString name;
    IsOnlineEnum isOnline;

    void setChannel(const QString& channel);

    Trackable(const QString &stringRepresentation);
};

#endif // TRACKABLE_H
