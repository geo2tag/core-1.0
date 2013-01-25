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
    qDebug() << "Initializing MetaCache objects";
    initUsers();
}

BasicUser MetaCache::getUserById(const QString userId)
{
    QReadLocker lock(&s_usersLock);

    QSharedPointer<User> realUser;      // Null pointer

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

void MetaCache::reloadSessions()
{
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
    qDebug() << "check channel " << channel.getName() << " for " << user;
    NOT_IMPLEMENTED();
    return true;
}

bool MetaCache::writeTag(const Tag &tag)
{
    return QueryExecutor::instance()->insertNewTag(tag);
}

void MetaCache::initUsers()
{
    QWriteLocker lock(&s_cacheLock);

    qDebug() << "Initializing Users";
    s_users=QueryExecutor::instance()->loadUsers();
    qDebug() << "Loaded " << s_users.size() << "users";
}

void MetaCache::initSessions()
{
    NOT_IMPLEMENTED();
}

} // namespace Core
