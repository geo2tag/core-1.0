#ifndef CHANNELSINTERSECTIONRESPONSEJSON_H
#define CHANNELSINTERSECTIONRESPONSEJSON_H

#include "ChannelsOperationResponseJSON.h"

class ChannelsIntersectionResponseJSON: public ChannelsOperationResponseJSON
{
public:
    ChannelsIntersectionResponseJSON(QObject *parent=0);
    QByteArray getJson() const;
    virtual bool parseJson(const QByteArray &) { return true;}
};

#endif // CHANNELSINTERSECTIONRESPONSEJSON_H
