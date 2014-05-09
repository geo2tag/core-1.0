#ifndef CHANNELSINTERSECTIONREQUESTJSON_H
#define CHANNELSINTERSECTIONREQUESTJSON_H

#include "ChannelsOperationRequestJSON.h"

class ChannelsIntersectionRequestJSON: public ChannelsOperationRequestJSON
{
public:
    ChannelsIntersectionRequestJSON(QObject *parent);
    bool parseJson(const QByteArray &);
    virtual QByteArray getJson() const { return ChannelsOperationRequestJSON::getJson(); }
};

#endif // CHANNELSINTERSECTIONREQUESTJSON_H
