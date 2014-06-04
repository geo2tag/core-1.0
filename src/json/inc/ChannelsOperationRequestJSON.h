#ifndef CHANNELSOPERATIONREQUESTJSON_H
#define CHANNELSOPERATIONREQUESTJSON_H

#include "JsonSerializer.h"
#include <QStringList>

class ChannelsOperationRequestJSON: public JsonSerializer
{
    QStringList formula;
    double latitude;
    double longitude;
    double radius;
    int quantity;
public:
    ChannelsOperationRequestJSON(QObject *parent=0);

    virtual bool parseJson(const QByteArray &data);
    virtual QByteArray getJson() const { QByteArray a; return a;}
    QStringList getFormula() const;
    double getLatitude() const;
    double getLongitude() const;
    double getRadius() const;
    int getQuantity() const;
};

#endif // CHANNELSOPERATIONREQUESTJSON_H
