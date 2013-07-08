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
 * \file DbSession.h
 * \brief Header of DbSession
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _DbSession_H_9BF6A8FE_DA47_4F7A_B008_2EA2842C490F_INCLUDED_
#define _DbSession_H_9BF6A8FE_DA47_4F7A_B008_2EA2842C490F_INCLUDED_

#include "servicelogger.h"

#include <QtSql>
#include <QThread>
#include <QMap>
#include "DataMarks.h"
#include "Channel.h"
#include "User.h"
#include "UpdateThread.h"
#include "QueryExecutor.h"
#include "FilterRequestJSON.h"
#include "Session.h"

namespace Core{
    class MetaCache;
}


namespace common
{

  /*!
   * \brief session with database
   */
  class DbObjectsCollection
  {

    QList<Channel>     m_channelsContainer;
    QList<Tag>    m_tagsContainer;
//    QSharedPointer<QList<tag>> m_QList<Tag>Map;

    typedef QByteArray (DbObjectsCollection::*ProcessMethod)(const QByteArray&);
    typedef QByteArray (DbObjectsCollection::*ProcessMethodWithStr)(const QString&);

    QMap<QString, ProcessMethod> m_processors;

    static const QString error;
    static const QString ok;

    Core::MetaCache * m_defaultCache;


    DbObjectsCollection();

    // initializes db by default sql script
    void autoInitdatabase();

    const QString getPasswordHash(const QString & login, const QString & pasword) const;
    const QString getPasswordHash(const common::BasicUser& user)  const;

    bool checkPasswordQuality(const QString& password) const;

    const QString generateNewPassword(const common::BasicUser& user) const;

    common::BasicUser findUser(const common::BasicUser &dummyUser) const;

    bool areCredentialsIncorrect(const BasicUser& realUser, const BasicUser& user) const;

    QByteArray processRegisterUserQuery(const QByteArray&);
    QByteArray processConfirmRegistrationQuery(const QString&);
    QByteArray processLoginQuery(const QByteArray&);
    QByteArray processQuitSessionQuery(const QByteArray&);
    QByteArray processOwnedChannelsQuery(const QByteArray&);
    QByteArray processSubscribedChannelsQuery(const QByteArray&);
    QByteArray processAvailableChannelsQuery(const QByteArray&);
    QByteArray processWriteTagQuery(const QByteArray&);
    QByteArray processLoadTagsQuery(const QByteArray&);
    QByteArray processSubscribeQuery(const QByteArray&);
    QByteArray processUnsubscribeQuery(const QByteArray&);
    QByteArray processAddUserQuery(const QByteArray&);
    QByteArray processAddChannelQuery(const QByteArray&);
    QByteArray processVersionQuery(const QByteArray&);
    QByteArray processDeleteUserQuery(const QByteArray&);
    QByteArray processBuildQuery(const QByteArray&);

    QByteArray processGetErrnoInfo(const QByteArray&);

    QByteArray processFilterCircleQuery(const QByteArray&);
    QByteArray processFilterCylinderQuery(const QByteArray&);
    QByteArray processFilterPolygonQuery(const QByteArray&);
    QByteArray processFilterRectangleQuery(const QByteArray&);
    QByteArray processFilterBoxQuery(const QByteArray&);
    QByteArray processFilterFenceQuery(const QByteArray&);
    QByteArray processFilterChannelQuery(const QByteArray&);

    QByteArray processRestorePasswordQuery(const QByteArray&);

    QByteArray internalProcessFilterQuery(FilterRequestJSON&, const QByteArray&, bool is3d);


    QByteArray processSetDbQuery(const QByteArray &data);
    //    void processSendConfirmationLetter(const QString &address);

    //    static void processSendConfirmationLetter(const QString&);

    void extractLastNTags(QList<Tag>& tags, qulonglong tagNumber);

    public:

      static DbObjectsCollection& getInstance();
      static void init();

      QByteArray process(const QString& queryType, const QByteArray& body);

      ~DbObjectsCollection();

      static QString getPlatformVersion();
      static QString getPlatformBuildInfo();


  private:
      DbObjectsCollection(const DbObjectsCollection& obj);
      DbObjectsCollection& operator=(const DbObjectsCollection& obj);

  };                                    // class DbSession

  // namespace common
}
#endif                                  //_DbSession_H_9BF6A8FE_DA47_4F7A_B008_2EA2842C490F_INCLUDED_

/* ===[ End of file ]=== */
