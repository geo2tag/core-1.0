#ifndef GETBLOBREQUESTJSON_H
#define GETBLOBREQUESTJSON_H

#include "JsonSerializer.h"


class GetBlobRequestJSON : public JsonSerializer
{
private:
    QString m_guid;
public:

    GetBlobRequestJSON(QObject *parent=0);

    virtual QByteArray getJson() const;

    virtual bool parseJson(const QByteArray&);

    void setGuid(const QString & guid);
    QString getGuid();
};

#endif // GETBLOBREQUESTJSON_H
