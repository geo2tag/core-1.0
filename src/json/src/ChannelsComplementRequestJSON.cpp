#include "ChannelsComplementRequestJSON.h"

ChannelsComplementRequestJSON::ChannelsComplementRequestJSON(QObject *parent): ChannelsOperationRequestJSON(parent)
{
}

bool ChannelsComplementRequestJSON::parseJson(const QByteArray &data)
{
    return ChannelsOperationRequestJSON::parseJson(data);
}
