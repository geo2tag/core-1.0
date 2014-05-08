#ifndef CHANNELINTERSECTIONRESPONSEJSON_H
#define CHANNELINTERSECTIONRESPONSEJSON_H

#include "JsonSerializer.h"

class ChannelsIntersectionResponseJSON: public JsonSerializer
{
public:
    ChannelsIntersectionResponseJSON(QObject *parent=0);

    virtual QByteArray getJson() const;

    virtual bool parseJson(const QByteArray&);
};

#endif // CHANNELINTERSECTIONRESPONSEJSON_H
