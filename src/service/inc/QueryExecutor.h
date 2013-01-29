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

#ifndef QUERYEXECUTOR_H
#define QUERYEXECUTOR_H

#include <QObject>

#include "User.h"
#include "Channel.h"
#include "DataMarks.h"
#include "Session.h"
#include "Geo2tagDatabase.h"
#include <QList>
#include <QSqlQuery>

class QueryExecutor : public QObject
{
    Q_OBJECT

    //Geo2tagDatabase m_database;

    qlonglong nextKey(const QString& sequence) const;
    QueryExecutor();

public:


    qlonglong nextUserKey() const;
    qlonglong nextChannelKey() const;
    qlonglong nextTagKey() const;
    qlonglong nextSessionKey() const;

    const QString generateNewToken(const QString& email, const QString& login,const QString& password) const;
    const QString generateNewToken(const QString& accessTime, const QString& email, const QString& login,const QString& password) const;

    bool                     subscribeChannel(const common::BasicUser& user,const Channel& channel);
    bool                     unsubscribeChannel(const common::BasicUser& user,const Channel& channel);
    bool                 doesTmpUserExist(const common::BasicUser &user);
    bool                     doesUserWithGivenEmailExist(const common::BasicUser &user);
    bool                     deleteTmpUser(const common::BasicUser &user);
    bool  insertNewTmpUser(const common::BasicUser &user);
    bool                     doesRegistrationTokenExist(const QString &token);
    common::BasicUser insertTmpUserIntoUsers(const QString &token);
    bool                     deleteTmpUser(const QString &token);
    bool insertNewTag(const Tag& tag);
    common::BasicUser    insertNewUser(const common::BasicUser&);
    bool  insertNewChannel(const Channel &, const common::BasicUser& user);
    bool                     deleteUser(const common::BasicUser &user);
    common::BasicUser  updateUserPassword(common::BasicUser &user, const QString& password);
    // Sessions
    bool insertNewSession(const Session &session);

    bool                     updateSession(Session& session);
    bool                     deleteSession(const Session& session);

    void checkTmpUsers();
    //void checkSessions(UpdateThread* thread);

    QList<common::BasicUser> loadUsers();
    QList<Channel> getChannelsByUser(const common::BasicUser& user );
    QList<Tag> loadTags();
    QList<Tag> loadTags(const Channel &channel);

    QList<Channel> loadChannels();
    QList<Session> loadSessions();
    void updateReflections(DataMarks&, QList<common::BasicUser>&, Channels&, Sessions&);
    qlonglong getUserIdByName(const QString& name);
    qlonglong getChannelIdByName(const QString& name);


    qlonglong getFactTransactionNumber();

    static QSqlQuery makeQuery();
    static void transaction();
    static void rollback();
    static void commit();
    static QueryExecutor* instance();

signals:

public slots:

};
#endif                                  // QUERYEXECUTOR_H
