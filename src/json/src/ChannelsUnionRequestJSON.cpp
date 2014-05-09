#include "ChannelsUnionRequestJSON.h"

ChannelsUnionRequestJSON::ChannelsUnionRequestJSON(QObject *parent): ChannelsOperationRequestJSON(parent)
{
}

bool ChannelsUnionRequestJSON::parseJson(const QByteArray &data)
{
    return ChannelsOperationRequestJSON::parseJson(data);
}
