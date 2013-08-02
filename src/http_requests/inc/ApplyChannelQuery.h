#ifndef APPLYCHANNELQUERY_H
#define APPLYCHANNELQUERY_H
#include <QObject>
#include <QString>
#include "DefaultQuery.h"
#include "Session.h"
#include "Channel.h"

class ApplyChannelQuery: public DefaultQuery
{
  Q_OBJECT

  Channel m_channel;
  Session m_session;

  virtual QString getUrl() const;
  virtual QByteArray getRequestBody() const;

  public:

    ApplyChannelQuery(const Channel &channel, const Session &session, QObject *parent = 0);

    explicit ApplyChannelQuery(QObject *parent = 0);

    void setQuery(const Channel &channel, const Session &session);

    Channel getChannel() const;

    ~ApplyChannelQuery();

};
#endif                                  // APPLYCHANNELQUERY_H
