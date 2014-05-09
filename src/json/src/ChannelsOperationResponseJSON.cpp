#include "ChannelsOperationResponseJSON.h"

#include <qjson/serializer.h>
#include <qjson/parser.h>
#include "ErrnoTypes.h"

ChannelsOperationResponseJSON::ChannelsOperationResponseJSON(QObject *parent): JsonSerializer(parent)
{
}

QByteArray ChannelsOperationResponseJSON::getJson() const
{
    QJson::Serializer serializer;
    QVariantMap obj;

    obj.insert("errno", m_errno);
    if (getErrno()== SUCCESS)
        obj.insert("operation", "yes");
    return serializer.serialize(obj);
}

void ChannelsOperationResponseJSON::setTags(QList<Tag> &)
{
}


