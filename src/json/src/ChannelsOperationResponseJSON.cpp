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
    QVariantList jtags;

    for(int j=0; j<m_tags.size(); j++)
    {
      Tag tag = m_tags.at(j);
      QVariantMap jtag;
      jtag["title"] = tag.getLabel();
      jtag["link"] = tag.getUrl();
      jtag["description"] = tag.getDescription();
      jtag["latitude"] = tag.getLatitude();
      jtag["longitude"] = tag.getLongitude();
      jtag["altitude"] = tag.getAltitude();
      jtag["user"] = tag.getUser().getLogin();
      jtag["pubDate"] = tag.getTime().toString("dd MM yyyy HH:mm:ss.zzz");
      jtags.append(jtag);
    }



    obj.insert("errno", m_errno);
    if (getErrno()== SUCCESS)
        obj.insert("tags", jtags);
    return serializer.serialize(obj);
}

void ChannelsOperationResponseJSON::setTags(const QList<Tag> &tags)
{
    Tag t;
    foreach(t, tags)
    {
      m_tags.push_back(t);
    }
}


