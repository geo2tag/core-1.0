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

#include "SetBlobQuery.h"
#include "defines.h"
#include "SetBlobRequestJSON.h"
#include "SetBlobResponseJSON.h"

SetBlobQuery::SetBlobQuery(const Session& session,
                           const QString &guid,
                           const QString &blob, QObject *parent)
    : DefaultQuery(parent),
      m_session(session),
      m_guid(guid),
      m_blob(blob)
{

}


SetBlobQuery::SetBlobQuery(QObject *parent): DefaultQuery(parent)
{
}


void SetBlobQuery::setBlob(const QString &blob)
{
    m_blob = blob;
}


QString SetBlobQuery::getUrl() const
{
    return SET_BLOB_HTTP_URL;
}


QByteArray SetBlobQuery::getRequestBody() const
{
    SetBlobRequestJSON request;
    request.setGuid(m_guid);
    request.setBlob(m_blob);
    request.setSessionToken(m_session.getSessionToken());
    return request.getJson();
}


SetBlobQuery::~SetBlobQuery()
{

}


QString SetBlobQuery::getBlob() const
{
    return m_blob;
}


void SetBlobQuery::setSession(const Session &session)
{
    m_session = session;
}


Session SetBlobQuery::getSession() const
{
    return m_session;
}


void SetBlobQuery::setGuid(const QString &guid)
{
    m_guid = guid;
}


QString SetBlobQuery::getGuid()
{
    return m_guid;
}

