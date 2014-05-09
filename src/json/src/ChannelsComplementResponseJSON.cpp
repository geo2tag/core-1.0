#include "ChannelsComplementResponseJSON.h"

ChannelsComplementResponseJSON::ChannelsComplementResponseJSON(QObject *parent): ChannelsOperationResponseJSON(parent)
{
}

QByteArray ChannelsComplementResponseJSON::getJson() const
{
    return ChannelsOperationResponseJSON::getJson();
}
