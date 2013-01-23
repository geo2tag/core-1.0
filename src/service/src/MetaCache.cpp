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

namespace Core
{

Channels            MetaCache::s_channels;
Sessions            MetaCache::s_sessions;
Users               MetaCache::s_users;

QReadWriteLock      MetaCache::s_cacheLock;
QReadWriteLock      MetaCache::s_usersLock;
QReadWriteLock      MetaCache::s_channelsLock;
QReadWriteLock      MetaCache::s_SessionsLock;

void MetaCache::init()
{
    qDebug() << "Initializing MetaCache objects";
}

QSharedPointer<User> MetaCache::getUserById(const QString userId)
{
    QReadLocker lock(&s_usersLock);

    QSharedPointer<User> realUser;      // Null pointer
    QVector<QSharedPointer<User> > currentUsers = s_users.vector();
    qDebug() << "checking user key: " << userId << "from "<< currentUsers.size() << " known users";

    if (!userId.isEmpty())
    {
        for(int i=0; i<currentUsers.size(); i++)
        {
            if(QString::compare(currentUsers.at(i)->getEmail(), userId, Qt::CaseInsensitive) == 0)
                return currentUsers.at(i);
        }
    }
    return realUser;
}

QVector<QSharedPointer<User> > MetaCache::getUsers()
{
    QReadLocker lock(&s_usersLock);
    return   s_users.vector();
}

void MetaCache::initUsers()
{
    QWriteLocker lock(&s_cacheLock);

    qDebug() << "Initializing Users";
    QueryExecutor::instance()->loadUsers(s_users);
    qDebug() << "Loaded " << s_users.size() << "users";
}

} // namespace Core
