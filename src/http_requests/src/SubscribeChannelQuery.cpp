#include "SubscribeChannelQuery.h"
#include "defines.h"
#include "SubscribeChannelRequestJSON.h"
#include "SubscribeChannelResponseJSON.h"

SubscribeChannelQuery::SubscribeChannelQuery(const Channel &channel,
const Session &session, QObject *parent)
: DefaultQuery(parent),
m_channel(channel),
m_session(session)
{

}


SubscribeChannelQuery::SubscribeChannelQuery(QObject *parent): DefaultQuery(parent)
{
}


void SubscribeChannelQuery::setQuery(const Channel &channel, const Session &session)
{
  m_channel = channel;
  m_session = session;
}


QString SubscribeChannelQuery::getUrl() const
{
  return SUBSCRIBE_HTTP_URL;
}


QByteArray SubscribeChannelQuery::getRequestBody() const
{
  SubscribeChannelRequestJSON request;
  request.addChannel(m_channel);
  request.setSessionToken(m_session.getSessionToken());
  return request.getJson();
}


SubscribeChannelQuery::~SubscribeChannelQuery()
{

}
