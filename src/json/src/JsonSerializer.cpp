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

#include "JsonSerializer.h"
#include "MetaCache.h"

JsonSerializer::JsonSerializer(QObject * parent)
    : QObject(parent)
{
}


JsonSerializer::~JsonSerializer()
{

}


QList<common::BasicUser> JsonSerializer::getUsers() const
{
    return m_usersContainer;
}

common::BasicUser JsonSerializer::getUser() const
{
    return
            m_usersContainer.size()>0
                ? m_usersContainer.at(0)
                : common::BasicUser();
}


QList<Channel> JsonSerializer::getChannels() const
{
    return m_channelsContainer;
}

QList<Tag> JsonSerializer::getTags() const
{
    return m_tagsContainer;
}

Tag JsonSerializer::getTag() const
{

    return m_tagsContainer.isEmpty() ? Tag() : m_tagsContainer.at(0);
}

QString JsonSerializer::getSessionToken() const
{
    return m_sessionToken;
}

Channel JsonSerializer::getChannel() const
{
    return m_channelsContainer.isEmpty() ? Channel() : m_channelsContainer.at(0);
}


void JsonSerializer::addChannel(const Channel &channel)
{
    m_channelsContainer.push_back(channel);
}


void JsonSerializer::addTag(const Tag &tag)
{
    m_tagsContainer.push_back(tag);
}


void JsonSerializer::addUser(const common::BasicUser &user)
{
    m_usersContainer.push_back(user);
}


void JsonSerializer::setSessionToken(const QString& token)
{
    m_sessionToken=token;
}


void JsonSerializer::clearContainers()
{
    m_usersContainer.clear();
    m_tagsContainer.clear();
    m_channelsContainer.clear();
}


const QString& JsonSerializer::getStatus() const
{
    return m_status;
}


const QString& JsonSerializer::getStatusMessage() const
{
    return m_statusMessage;
}


void JsonSerializer::setStatus(const QString &s)
{
    m_status = s;
}


void JsonSerializer::setStatusMessage(const QString &s)
{
    m_statusMessage = s;
}


const int& JsonSerializer::getErrno() const
{
    return m_errno;
}


void JsonSerializer::setErrno(const int &s)
{
    m_errno = s;
}
