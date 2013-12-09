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
 * \file FilterSubstringRequestJSON.cpp
 * \brief FilterSubstringRequestJSON implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "FilterSubstringRequestJSON.h"

#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <QVariant>
#include <QVariantMap>
#include <QDebug>

#include "DataMarks.h"
#include "JsonChannel.h"
#include "JsonDataMark.h"
#include "JsonSession.h"
#include "servicelogger.h"


FilterSubstringRequestJSON::FilterSubstringRequestJSON(QObject *parent) : JsonSerializer(parent),
	m_tagNumber(0)
{
  DEBUG() << "FilterSubstringRequestJSON::FilterSubstringRequestJSON()";
}

bool FilterSubstringRequestJSON::parseJson(const QByteArray &data)
{
  clearContainers();

  QJson::Parser parser;
  bool ok;
  QVariantMap result = parser.parse(data, &ok).toMap();

  if(!ok) return false;

  QString field = result["field"].toString();
  QString substring = result["substring"].toString();
  QString auth_token = result["auth_token"].toString();
  qulonglong tagNumber = result["tag_number"].toULongLong(&ok);
  if (ok) m_tagNumber = tagNumber;

  if ( !Tag::isFieldNameValid(field) || substring.isEmpty() || auth_token.isEmpty())
    return false;

  setField(field);
  setSubstring(substring);
  setSessionToken(auth_token);

  return true;
}


QByteArray FilterSubstringRequestJSON::getJson() const
{
  QJson::Serializer serializer;
  QVariantMap request;

  request.insert("auth_token", m_token);
  request.insert("field", m_field);
  request.insert("substring", m_substring);
  request.insert("tag_number", m_tagNumber);
  

  return serializer.serialize(request);
}

void FilterSubstringRequestJSON::setField(const QString& field)
{
  m_field = field;
}

const QString FilterSubstringRequestJSON::getField() const
{
  return m_field;
} 

void FilterSubstringRequestJSON::setSubstring(const QString& substring)
{
  m_substring = substring;
}

const QString FilterSubstringRequestJSON::getSubstring() const
{
  return m_substring;
} 

void FilterSubstringRequestJSON::setTagNumber(qulonglong tagNumber)
{
  m_tagNumber = tagNumber;
}

qulonglong FilterSubstringRequestJSON::getTagNumber() const
{
  return m_tagNumber;
} 

/* ===[ End of file :$HeadURL$ ]=== */
