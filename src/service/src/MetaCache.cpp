/*
 * Copyright 2013  Kirill Krinkin  kirill.krinkin@gmail.com
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
 * \file MetaCache.cpp
 * \brief MetaCache implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "MetaCache.h"
#include "QueryExecutor.h"
#include "servicelogger.h"
#include "defines.h"

namespace Core
{

QList<Channel>           MetaCache::s_channels;
QList<Session>           MetaCache::s_sessions;
QList<BasicUser>         MetaCache::s_users;

QReadWriteLock      MetaCache::s_cacheLock;
QReadWriteLock      MetaCache::s_usersLock;
QReadWriteLock      MetaCache::s_channelsLock;
QReadWriteLock      MetaCache::s_SessionsLock;

void MetaCache::init()
{
    DEBUG() << "Initializing MetaCache objects";
    initUsers();
    initChannels();
}

BasicUser MetaCache::getUserById(const QString userId)
{
    QReadLocker lock(&s_usersLock);

    common::BasicUser realUser;      // Null pointer

    BasicUser user;
    foreach(user,s_users)
    {
        if(QString::compare(user.getEmail(), userId, Qt::CaseInsensitive) == 0)
            return user;
    }

    return BasicUser();
}

QList<BasicUser> MetaCache::getUsers()
{
    QReadLocker lock(&s_usersLock);
    return   s_users;
}

QList<Channel> MetaCache::getChannels()
{
    QReadLocker lock(&s_channelsLock);
    return   s_channels;
}

void MetaCache::addChannel(const Channel &channel, const common::BasicUser& user)
{
    QueryExecutor::instance()->insertNewChannel(channel, user);
    s_channels.push_back(Channel(channel));
}

void MetaCache::insertSession(const Session& session)
{
    s_sessions << session;
    DEBUG() << "inserted " << session << ", session count=" << s_sessions.size();
}

Channel MetaCache::getChannel(const QString name)
{
    return QueryExecutor::instance()->getChannel(name);
}

Session MetaCache::findSession(const BasicUser &user)
{
    QReadLocker lock(&s_SessionsLock);

    Session s;
    foreach(s,s_sessions)
    {
        if(s.getUser() == user)
            return s;
    }
    return Session();
}

Session MetaCache::findSession(const QString &token)
{
    DEBUG() << "Looking op for a session, token " << token;
    Session s;
    foreach(s,s_sessions)
    {
        DEBUG() << "test:" << s;
        if(s.getSessionToken() == token )
        {

            DEBUG() << "...found " << s;
            return s;
        }
    }
    DEBUG() << "...not found ";
    return Session();
}

Channel MetaCache::findChannel(const QString &name)
{
#ifdef GEO2TAG_LITE
    return QueryExecutor::instance()->getChannel(name);
#else
    Channel ch;
    QReadLocker lock(&s_channelsLock);
    foreach(ch,s_channels)
    {
        if(ch.getName() == name)
            return ch;
    }
    return Channel();
#endif
}

bool MetaCache::subscribeChannel(const BasicUser &user, const Channel &channel)
{
    return QueryExecutor::instance()->subscribeChannel(user,channel);
}

bool MetaCache::unsubscribeChannel(const BasicUser &user, const Channel &channel)
{
    return QueryExecutor::instance()->unsubscribeChannel(user,channel);
}

void MetaCache::reloadSessions()
{
    QWriteLocker lock(&s_SessionsLock);
    initSessions();
}

bool MetaCache::checkUser(BasicUser &user)
{
    QReadLocker lock(&s_usersLock);

    common::BasicUser u;
    foreach(u,s_users)
    {
        if(u==user)
            return true;
    }
    return false;
}

bool MetaCache::testChannel(BasicUser &user, const Channel& channel)
{
    DEBUG() << "check channel " << channel.getName() << " for " << user.getLogin();

    Channel ch = Core::MetaCache::findChannel(channel.getName());
    if(!ch.isValid())
    {
        WARNING() << "Channel " << channel << " is invalid";
        return false;
    }

#ifdef GEO2TAG_LITE
    return true;
#else
    //TODO check user rights
    NOT_IMPLEMENTED();
    return true;
#endif
}

bool MetaCache::writeTag(const Tag &tag)
{
    return QueryExecutor::instance()->insertNewTag(tag);
}

void MetaCache::initUsers()
{
    QWriteLocker lock(&s_cacheLock);

    DEBUG() << "Initializing Users";
    s_users=QueryExecutor::instance()->loadUsers();
    DEBUG() << "Loaded " << s_users.size() << "users";
}

void MetaCache::initSessions()
{
#if GEO2TAG_LITE
    DEBUG() << "Session persistans is not supported in lite version";
#else
    NOT_IMPLEMENTED();
#endif
}

void MetaCache::initChannels()
{
    QWriteLocker lockf(&s_channelsLock);
#ifdef GEO2TAG_LITE
    // initialization is not supported;
#else
    NOT_IMPLEMENTED();
#endif
}

} // namespace Core
