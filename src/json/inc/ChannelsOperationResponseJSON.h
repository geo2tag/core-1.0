#ifndef CHANNELINTERSECTIONRESPONSEJSON_H
#define CHANNELINTERSECTIONRESPONSEJSON_H

#include "JsonSerializer.h"

class ChannelsOperationResponseJSON: public JsonSerializer
{
public:
    ChannelsOperationResponseJSON(QObject *parent=0);

    virtual QByteArray getJson() const;

    virtual bool parseJson(const QByteArray &) {return true;}

    void setTags(const QList<Tag> &tags);

};

#endif // CHANNELINTERSECTIONRESPONSEJSON_H
