/*
 * Copyright 2012  bac1ca  bac1ca89@gmail.com
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
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
 *
 * \file FilterChannelRequestJSON.cpp
 * \brief FilterChannelRequestJSON implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "FilterChannelRequestJSON.h"

#include "JsonSession.h"

#if !defined(Q_OS_SYMBIAN) && !defined(Q_WS_SIMULATOR)
#include <qjson/parser.h>
#include <qjson/serializer.h>
#else
#include "parser.h"
#include "serializer.h"
#endif


FilterChannelRequestJSON::FilterChannelRequestJSON(QObject *parent) : JsonSerializer(parent)
{
}


QByteArray FilterChannelRequestJSON::getJson() const
{
  // TODO TBD
  QJson::Serializer serializer;
  QVariantMap obj;
  obj.insert("auth_token", m_token);
  obj.insert("channel", m_token);
  obj.insert("amount", m_token);
  return serializer.serialize(obj);
  return NULL;
}


bool FilterChannelRequestJSON::parseJson(const QByteArray&data)
{
  clearContainers();

  QJson::Parser parser;
  bool ok;
  QVariantMap result = parser.parse(data, &ok).toMap();
  if (!ok) return false;

  QString auth_token = result["auth_token"].toString();
  if (auth_token.isEmpty()) return false;

  m_token = auth_token;

  m_channel = result["channel"].toString();
  m_amount = result["amount"].toUInt(&ok);
  return ok;
}

void FilterChannelRequestJSON::setChannelName(const QString& name)
{
  m_channel = name;
}

void FilterChannelRequestJSON::setAmount(unsigned int amount)
{
  m_amount = amount;
}


QString FilterChannelRequestJSON::getChannelName()
{
  return m_channel;
}


unsigned int FilterChannelRequestJSON::getAmount()
{
  return m_amount;
}
