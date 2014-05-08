#include "ChannelsIntersectionResponseJSON.h"

#include <qjson/serializer.h>
#include <qjson/parser.h>
#include "ErrnoTypes.h"

ChannelsIntersectionResponseJSON::ChannelsIntersectionResponseJSON(QObject *parent): JsonSerializer(parent)
{
}

bool ChannelsIntersectionResponseJSON::parseJson(const QByteArray &data)
{
    clearContainers();
    QJson::Parser parser;
    bool ok;
    QVariantMap result = parser.parse(data, &ok).toMap();
    if (!ok)
    {
      return false ;
    }
    m_errno = result["errno"].toInt();
    return true;
}


QByteArray ChannelsIntersectionResponseJSON::getJson() const
{
    QJson::Serializer serializer;
    QVariantMap obj;

    obj.insert("errno", m_errno);
    if (getErrno()== SUCCESS)
        obj.insert("success", "yes");
    return serializer.serialize(obj);
}


