/*
 * Copyright 2013  Open Source & Linux Lab (OSLL)  osll@osll.spb.ru
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
 * \file AlterChannelRequestJSON.cpp
 * \brief AlterChannelRequestJSON implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "AlterChannelRequestJSON.h"

#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <QVariant>
#include <QVariantMap>
#include <QDebug>

#include "JsonChannel.h"
#include "JsonDataMark.h"
#include "JsonSession.h"
#include "servicelogger.h"


AlterChannelRequestJSON::AlterChannelRequestJSON(QObject *parent) : JsonSerializer(parent)
{
  DEBUG() << "AlterChannelRequestJSON::AlterChannelRequestJSON()";
}

bool AlterChannelRequestJSON::parseJson(const QByteArray &data)
{
  clearContainers();

  QJson::Parser parser;
  bool ok;
  QVariantMap result = parser.parse(data, &ok).toMap();

  if(!ok) return false;

  QString field = result["field"].toString();
  QString value = result["value"].toString();
  QString name = result["name"].toString();
  QString auth_token = result["auth_token"].toString();

  if ( !Channel::isFieldNameValid(field) || value.isEmpty() || name.isEmpty() || auth_token.isEmpty()) return false;

  setField(field);
  setValue(value);
  setSessionToken(auth_token);
  setName(name);

  return true;
}


QByteArray AlterChannelRequestJSON::getJson() const
{
  QJson::Serializer serializer;
  QVariantMap request;

  request.insert("auth_token", m_token);
  request.insert("field", m_field);
  request.insert("value", m_value);
  request.insert("name", m_name);

  return serializer.serialize(request);
}

void AlterChannelRequestJSON::setField(const QString& field)
{
  m_field = field;
}

const QString AlterChannelRequestJSON::getField() const
{
  return m_field;
} 

void AlterChannelRequestJSON::setValue(const QString& value)
{
  m_value = value;
}

const QString AlterChannelRequestJSON::getValue() const
{
  return m_value;
} 

void AlterChannelRequestJSON::setName(const QString& name)
{
  m_name = name;
}

const QString AlterChannelRequestJSON::getName() const
{
  return m_name;
} 
/* ===[ End of file $HeadURL$ ]=== */
