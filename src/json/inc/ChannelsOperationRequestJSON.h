#ifndef CHANNELSOPERATIONREQUESTJSON_H
#define CHANNELSOPERATIONREQUESTJSON_H

#include "JsonSerializer.h"
#include <QStringList>

class ChannelsOperationRequestJSON: public JsonSerializer
{
    QStringList formula;
public:
    ChannelsOperationRequestJSON(QObject *parent=0);

    virtual bool parseJson(const QByteArray &data);
    virtual QByteArray getJson() const { QByteArray a; return a;}
    QStringList getFormula() const;
};

#endif // CHANNELSOPERATIONREQUESTJSON_H
