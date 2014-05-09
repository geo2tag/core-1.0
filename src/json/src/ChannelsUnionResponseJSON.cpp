#include "ChannelsUnionResponseJSON.h"

ChannelsUnionResponseJSON::ChannelsUnionResponseJSON(QObject *parent): ChannelsOperationResponseJSON(parent)
{
}

QByteArray ChannelsUnionResponseJSON::getJson() const
{
    return ChannelsOperationResponseJSON::getJson();
}
