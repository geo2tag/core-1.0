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
 * \file SetDbRequestJSON.cpp
 * \brief SetDbRequestJSON implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "SetDbRequestJSON.h"

#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <QVariant>
#include <QVariantMap>
#include <QDebug>

#include "JsonChannel.h"
#include "JsonDataMark.h"
#include "JsonSession.h"
#include "servicelogger.h"


SetDbRequestJSON::SetDbRequestJSON(QObject *parent) : JsonSerializer(parent)
{
  DEBUG() << "SetDbRequestJSON::SetDbRequestJSON()";
}

bool SetDbRequestJSON::parseJson(const QByteArray &data)
{
  clearContainers();

  QJson::Parser parser;
  bool ok;
  QVariantMap result = parser.parse(data, &ok).toMap();

  if(!ok) return false;

  QString dbName = result["db_name"].toString();
  QString auth_token = result["auth_token"].toString();

  if (dbName.isEmpty() || auth_token.isEmpty()) return false;

  m_dbName = dbName;
  m_token = auth_token;
  return true;
}


QByteArray SetDbRequestJSON::getJson() const
{
  QJson::Serializer serializer;
  QVariantMap request;

  request.insert("auth_token", m_token);
  request.insert("db_name", m_dbName);

  return serializer.serialize(request);
}

void SetDbRequestJSON::setDbName(const QString& dbName)
{
  m_dbName = dbName;
}

const QString SetDbRequestJSON::getDbName() const
{
  return m_dbName;
} 

/* ===[ End of file $HeadURL$ ]=== */
