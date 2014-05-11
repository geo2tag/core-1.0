#include "ChannelsOperationRequestJSON.h"
#include "servicelogger.h"
#include <qjson/parser.h>
#include <qjson/serializer.h>


ChannelsOperationRequestJSON::ChannelsOperationRequestJSON(QObject *parent): JsonSerializer(parent)
{
}


bool ChannelsOperationRequestJSON::parseJson(const QByteArray &data)
{
    clearContainers();

    QJson::Parser parser;
    bool ok;
    QVariantMap result = parser.parse(data, &ok).toMap();

    if(!ok) return false;

    QString channel_1 = result["channel_1"].toString();
    QString channel_2 = result["channel_2"].toString();
    QString auth_token = result["auth_token"].toString();

    if(!channel_1.isEmpty())
        m_channels.push_back(Channel(channel_1));
    if(!channel_2.isEmpty())
        m_channels.push_back(Channel(channel_2));
    m_token = auth_token;
    return true;
}
