/*
 * Copyright 2010  Open Source & Linux Lab (OSLL)  osll@osll.spb.ru
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */

/*! ---------------------------------------------------------------
 * $Id$
 *
 * \file SubscribedChannelsQuery.cpp
 * \brief SubscribedChannelsQuery implementation
 *
 * File description
 *
 * PROJ: geo2tag
 * ---------------------------------------------------------------- */
#include "SubscribedChannelsQuery.h"
#include "defines.h"
#include <QDebug>
#include "SubscribedChannelsResponseJSON.h"
#include "SubscribedChannelsRequestJSON.h"
#include "JsonDataMark.h"
#include "JsonUser.h"

SubscribedChannelsQuery::SubscribedChannelsQuery(const Session& session, QObject *parent)
: DefaultQuery(parent),
m_session(session)
{
}


SubscribedChannelsQuery::SubscribedChannelsQuery(QObject *parent): DefaultQuery(parent)
{
}


void SubscribedChannelsQuery::setQuery(const Session &session)
{
  m_session = session;
}


QString SubscribedChannelsQuery::getUrl() const
{
  return SUBSCRIBED_LIST_HTTP_URL;
}


QByteArray SubscribedChannelsQuery::getRequestBody() const
{
  SubscribedChannelsRequestJSON request;
  request.setSessionToken(m_session.getSessionToken());
  return request.getJson();
}


void SubscribedChannelsQuery::processResponse(const QByteArray &data)
{
  SubscribedChannelsResponseJSON response;
  response.parseJson(data);
  m_channels = response.getChannels();
}


QList<Channel> SubscribedChannelsQuery::getChannels() const
{
  return m_channels;
}


SubscribedChannelsQuery::~SubscribedChannelsQuery()
{

}
