#include "ApplyChannelQuery.h"
#include "defines.h"
#include "AddChannelRequestJSON.h"
#include "AddChannelResponseJSON.h"

ApplyChannelQuery::ApplyChannelQuery(const Channel &channel, const Session &session, QObject *parent)
: DefaultQuery(parent),
m_channel(channel),
m_session(session)
{
}


ApplyChannelQuery::ApplyChannelQuery(QObject *parent): DefaultQuery(parent)
{
}


void ApplyChannelQuery::setQuery(const Channel &channel, const Session &session)
{
  m_channel = channel;
  m_session = session;
}


QString ApplyChannelQuery::getUrl() const
{
  return APPLY_CHANNEL_HTTP_URL;
}


QByteArray ApplyChannelQuery::getRequestBody() const
{
  AddChannelRequestJSON request;
  request.addChannel(m_channel);
  request.setSessionToken(m_session.getSessionToken());
  return request.getJson();
}


ApplyChannelQuery::~ApplyChannelQuery()
{

}


Channel ApplyChannelQuery::getChannel() const
{
  return m_channel;
}
