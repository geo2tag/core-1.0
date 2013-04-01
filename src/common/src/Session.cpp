/*
 * Copyright 2010  OSLL osll@osll.spb.ru
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
/*!
 * \file  Session.cpp
 * \brief Implementation of the class Session
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "Session.h"
#include <QCryptographicHash>
#include "servicelogger.h"

Session::Session(const QString &token, const QDateTime &accessTime, const common::BasicUser &user)
    : m_token(token), m_accessTime(accessTime), m_user(user)
{
}

Session::Session(const Session &obj)
{
    m_token = obj.m_token;
    m_accessTime = obj.m_accessTime;
    m_user = obj.m_user;
}

Session::Session() : m_token(""), m_accessTime(QDateTime::currentDateTime())
{
}

Session &Session::operator =(const Session &obj)
{
    m_token = obj.m_token;
    m_accessTime = obj.m_accessTime;
    m_user = obj.m_user;
    return *this;
}


void Session::setSessionToken(const QString &sessionToken)
{
    m_token = sessionToken;
}


void Session::setLastAccessTime(const QDateTime& lastAccessTime)
{
    m_accessTime = lastAccessTime;
}


void Session::setUser(const common::BasicUser &user)
{
    m_user = user;
}


const QString& Session::getSessionToken() const
{
    return m_token;
}


const QDateTime& Session::getLastAccessTime() const
{
    return m_accessTime;
}


common::BasicUser Session::getUser() const
{
    return m_user;
}

QString Session::generateToken(const common::BasicUser &user)
{
    QString token=user.getLogin()+user.getPassword()+user.getEmail()+QDateTime::currentDateTime().toString();
    QByteArray toHash(token.toUtf8());
    toHash=QCryptographicHash::hash(token.toUtf8(),QCryptographicHash::Md5);
    QString result(toHash.toHex());
    DEBUG() << "Generated token " << result << " for user " << user.getLogin();
    return result;
}

Session::~Session()
{
}

QDebug& operator<<(QDebug &dbg, Session const& session)
{
    dbg << session.getSessionToken() << " user:"
        << session.getUser() << " accessTime:"
        << session.getLastAccessTime() << ", valid=" << session.isValid();
    return dbg;
}
