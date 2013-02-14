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
 * \file SubscribeChannelJSON.cpp
 * \brief SubscribeChannelJSON implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "SubscribeChannelJSON.h"

#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <QVariant>
#include <QVariantMap>
#include <QDebug>

#include "JsonChannel.h"
#include "JsonDataMark.h"
#include "JsonSession.h"
#include "servicelogger.h"


SubscribeChannelRequestJSON::SubscribeChannelRequestJSON(QObject *parent) : JsonSerializer(parent)
{
  DEBUG() << "SubscribeChannelRequestJSON::SubscribeChannelRequestJSON()";
}

bool SubscribeChannelRequestJSON::parseJson(const QByteArray &data)
{
  clearContainers();

  QJson::Parser parser;
  bool ok;
  QVariantMap result = parser.parse(data, &ok).toMap();

  if(!ok) return false;

  QString channelLabel = result["channel"].toString();
  QString auth_token = result["auth_token"].toString();

  m_channels.push_back(Channel(channelLabel));

  m_token = auth_token;
  return true;
}


QByteArray SubscribeChannelRequestJSON::getJson() const
{
  QJson::Serializer serializer;
  QVariantMap request;

  request.insert("auth_token", m_token);
  if(m_channels.size()>0)
      request.insert("channel", m_channels.at(0).getName());
  else
  {
      WARNING() << "No channels in object";
  }

  return serializer.serialize(request);
}

/* ===[ End of file $HeadURL$ ]=== */
