/*
 * Copyright 2010-2011  OSLL osll@osll.spb.ru
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
/*----------------------------------------------------------------- !
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "WriteTagQuery.h"
#include "defines.h"
#include "WriteTagRequestJSON.h"
#include "WriteTagResponseJSON.h"

WriteTagQuery::WriteTagQuery(const Session& session,
const Channel& channel,
const Tag &tag, QObject *parent)
: DefaultQuery(parent),
m_session(session),
m_channel(channel),
m_tag(tag)
{

}


WriteTagQuery::WriteTagQuery(QObject *parent): DefaultQuery(parent)
{
}


void WriteTagQuery::setTag(const Tag &tag)
{
  m_tag = tag;
}


QString WriteTagQuery::getUrl() const
{
  return APPLY_HTTP_URL;
}


QByteArray WriteTagQuery::getRequestBody() const
{
  WriteTagRequestJSON request;
  request.addChannel(m_channel);
  request.addTag(m_tag);
  request.setSessionToken(m_session.getSessionToken());
  return request.getJson();
}


WriteTagQuery::~WriteTagQuery()
{

}


Tag WriteTagQuery::getTag()
{
  return m_tag;
}


Tag WriteTagQuery::getTag() const
{
  return m_tag;
}


void WriteTagQuery::setSession(const Session &session)
{
  m_session = session;
}


Session WriteTagQuery::getSession() const
{
  return m_session;
}


void WriteTagQuery::setChannel(const Channel &channel)
{
  m_channel = channel;
}


Channel WriteTagQuery::getChannel() const
{
  return m_channel;
}
