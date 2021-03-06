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

#include "SetBlobRequestJSON.h"
#include "servicelogger.h"

#if !defined(Q_OS_SYMBIAN) && !defined(Q_WS_SIMULATOR)
#include <qjson/parser.h>
#include <qjson/serializer.h>
#else
#include "parser.h"
#include "serializer.h"
#endif

SetBlobRequestJSON::SetBlobRequestJSON(QObject *parent) : JsonSerializer(parent)
{
}

void SetBlobRequestJSON::setGuid(const QString &guid)
{
    m_guid = guid;
}

QString SetBlobRequestJSON::getGuid()
{
    return m_guid;
}

void SetBlobRequestJSON::setBlob(const QString &blob)
{
    m_blob = blob;
}

QString SetBlobRequestJSON::getBlob()
{
    return m_blob;
}


QByteArray SetBlobRequestJSON::getJson() const
{
  QJson::Serializer serializer;
  QVariantMap obj;
  obj.insert("auth_token", m_token);
  obj.insert("guid", m_guid);
  obj.insert("blob", m_blob);
  return serializer.serialize(obj);
}

bool SetBlobRequestJSON::parseJson(const QByteArray &data)
{
    clearContainers();

    QJson::Parser parser;
    bool ok;
    QVariantMap result = parser.parse (data, &ok).toMap ();
    if (!ok)
        return false;

    QString auth_token = result["auth_token"].toString ();
    QString guid = result["guid"].toString ();
    QString blob = result["blob"].toString ();

    if (auth_token.isEmpty() || guid.isEmpty() || blob.isEmpty())
    {
      DEBUG() << "Some of the string fields are empty." << data;
      return false;
    }

    m_token = auth_token;
    m_blob = blob;
    m_guid = guid;
    return true;
}
