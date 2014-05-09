#ifndef CHANNELSCOMPLEMENTRESPONSEJSON_H
#define CHANNELSCOMPLEMENTRESPONSEJSON_H

#include "ChannelsOperationResponseJSON.h"

class ChannelsComplementResponseJSON: public ChannelsOperationResponseJSON
{
public:
    ChannelsComplementResponseJSON(QObject *parent=0);
    QByteArray getJson() const;
    virtual bool parseJson(const QByteArray &) { return true;}
};

#endif // CHANNELSCOMPLEMENTRESPONSEJSON_H
