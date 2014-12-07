#ifndef ADDTAGREQUESTJSON_H
#define ADDTAGREQUESTJSON_H

#include "JsonSerializer.h"
#include "Session.h"
#include "Channel.h"
#include "DataMarks.h"
#include "WriteTagRequestJSON.h"

class AddTagRequestJSON : public WriteTagRequestJSON
{
  public:

    AddTagRequestJSON(QObject *parent=0);

    AddTagRequestJSON(const Session &session,
      const Tag &tag,
      QObject *parent=0);

    virtual QByteArray getJson() const;

    virtual bool parseJson(const QByteArray&);
};

#endif // ADDTAGREQUESTJSON_H
