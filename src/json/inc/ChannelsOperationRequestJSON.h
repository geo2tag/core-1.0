#ifndef CHANNELSOPERATIONREQUESTJSON_H
#define CHANNELSOPERATIONREQUESTJSON_H

#include "JsonSerializer.h"

class ChannelsOperationRequestJSON: public JsonSerializer
{
public:
    ChannelsOperationRequestJSON(QObject *parent=0);

    virtual bool parseJson(const QByteArray &);
    virtual QByteArray getJson() const { QByteArray a; return a;}
};

#endif // CHANNELSOPERATIONREQUESTJSON_H
