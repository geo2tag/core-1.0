#ifndef CHANNELSCOMPLEMENTREQUESTJSON_H
#define CHANNELSCOMPLEMENTREQUESTJSON_H

#include "ChannelsOperationRequestJSON.h"

class ChannelsComplementRequestJSON: public ChannelsOperationRequestJSON
{
public:
    ChannelsComplementRequestJSON(QObject *parent);
    bool parseJson(const QByteArray &);
    virtual QByteArray getJson() const { return ChannelsOperationRequestJSON::getJson(); }

};

#endif // CHANNELSCOMPLEMENTREQUESTJSON_H
