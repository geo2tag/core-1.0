#ifndef CHANNELSUNIONREQUESTJSON_H
#define CHANNELSUNIONREQUESTJSON_H

#include "ChannelsOperationRequestJSON.h"

class ChannelsUnionRequestJSON: public ChannelsOperationRequestJSON
{
public:
    ChannelsUnionRequestJSON(QObject *parent);
    bool parseJson(const QByteArray &);
    virtual QByteArray getJson() const { return ChannelsOperationRequestJSON::getJson(); }
};

#endif // CHANNELSUNIONREQUESTJSON_H
