#include "ChannelsOperationRequestJSON.h"
#include "servicelogger.h"
#include <qjson/parser.h>
#include <qjson/serializer.h>


ChannelsOperationRequestJSON::ChannelsOperationRequestJSON(QObject *parent): JsonSerializer(parent)
{
}


bool ChannelsOperationRequestJSON::parseJson(const QByteArray &)
{
    return true;
}
