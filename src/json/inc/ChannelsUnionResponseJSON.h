#ifndef CHANNELSUNIONRESPONSEJSON_H
#define CHANNELSUNIONRESPONSEJSON_H

#include "ChannelsOperationResponseJSON.h"

class ChannelsUnionResponseJSON: public ChannelsOperationResponseJSON
{
public:
    ChannelsUnionResponseJSON(QObject *parent=0);
    QByteArray getJson() const;
    virtual bool parseJson(const QByteArray &) { return true;}
};

#endif // CHANNELSUNIONRESPONSEJSON_H
