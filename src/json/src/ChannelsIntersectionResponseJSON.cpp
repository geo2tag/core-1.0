#include "ChannelsIntersectionResponseJSON.h"

ChannelsIntersectionResponseJSON::ChannelsIntersectionResponseJSON(QObject *parent): ChannelsOperationResponseJSON(parent)
{
}


QByteArray ChannelsIntersectionResponseJSON::getJson() const
{
    return ChannelsOperationResponseJSON::getJson();
}
