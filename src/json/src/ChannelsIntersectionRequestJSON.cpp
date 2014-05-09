#include "ChannelsIntersectionRequestJSON.h"

ChannelsIntersectionRequestJSON::ChannelsIntersectionRequestJSON(QObject *parent): ChannelsOperationRequestJSON(parent)
{
}

bool ChannelsIntersectionRequestJSON::parseJson(const QByteArray &data)
{
    return ChannelsOperationRequestJSON::parseJson(data);
}
