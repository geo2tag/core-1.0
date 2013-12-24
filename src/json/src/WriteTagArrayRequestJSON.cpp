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

#include "WriteTagRequestJSON.h"
#include "DataMarks.h"
#include "JsonChannel.h"
#include "JsonDataMark.h"
#include "JsonSession.h"
#include "servicelogger.h"

#if !defined(Q_OS_SYMBIAN) && !defined(Q_WS_SIMULATOR)
#include <qjson/parser.h>
#include <qjson/serializer.h>
#else
#include "parser.h"
#include "serializer.h"
#endif

WriteTagRequestJSON::WriteTagRequestJSON(QObject *parent) : JsonSerializer(parent)
{
}


WriteTagRequestJSON::WriteTagRequestJSON(const Session &session,
	const QList<Tag> &tags,QObject *parent): JsonSerializer(parent)
{
  m_token = session.getSessionToken();
  for (int i=0; i<tags.size(); i++){
    m_channels.push_back(tags.at(i).getChannel());
  }
  m_tags.push_back << tags;
}


bool WriteTagRequestJSON::parseJson(const QByteArray &data)
{

  clearContainers();
  QJson::Parser parser;
  bool ok;
  QVariantMap result = parser.parse(data, &ok).toMap();
  if (!ok){
	DEBUG() << "General parsing error - invalid json." << data;
	return false;
  }


  QString auth_token = result["auth_token"].toString();

  if (auth_token.isEmpty()) 
  {
	DEBUG() << "AuthToken is empty." << data;
	return false;
  }

  m_token = auth_token;

  QVarianList tags = result["tags"].toList();
  for (int i = 0; i<tags.size(); i++)
  {
    // Parse each tag, check validity and add into m_tags
  }


  return true;
}


QByteArray WriteTagRequestJSON::getJson() const
{
  QJson::Serializer serializer;
  QVariantArray request;

  for (Tag tag: m_tags){
    // Generate json for each tag and add to request
  }

  return serializer.serialize(request);
}
