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

#ifndef UPDATETHREAD_H
#define UPDATETHREAD_H

#if 0
#include <QtSql>
#include <QThread>
#include <QReadWriteLock>
#include <QMap>
#include "DataMarkInternal.h"
#include "UserInternal.h"
#include "ChannelInternal.h"
#include "DataChannel.h"
#include "SessionInternal.h"

class QueryExecutor;

class UpdateThread: public QThread
{
  Q_OBJECT

    QList<Channel>     m_channelsContainer;
  QList<Tag>    m_tagsContainer;
  QList<common::BasicUser>        m_usersContainer;
  QSharedPointer<QList<Tag>> m_QList<Tag>Map;
  QSharedPointer<Sessions>     m_sessionsContainer;

  QueryExecutor* m_queryExecutor;

  //will be locked when containers is being updated
  QReadWriteLock m_updateLock;

  qlonglong m_transactionCount;

  bool compareTransactionNumber(qlonglong factCount);

  void run();

  public:

    UpdateThread(
      const QList<Tag>& tags,
      const QList<common::BasicUser>& users,
      const QList<Channel>& channels,
      const QSharedPointer<QList<Tag>>& QList<Tag>Map,
      const QSharedPointer<Sessions>& sessions,
      QObject *parent = 0);

    UpdateThread(
      const QList<Tag>& tags,
      const QList<common::BasicUser>& users,
      const QList<Channel>& channels,
      const QSharedPointer<QList<Tag>>& QList<Tag>Map,
      const QSharedPointer<Sessions>& sessions,
      QueryExecutor* queryExecutor,
      QObject *parent = 0);

    void incrementTransactionCount(int i = 1);

    QReadWriteLock* getLock();

    void setQueryExecutor(QueryExecutor* queryExecutor);
    QSharedPointer<Sessions> getSessionsContainer() const;

    signals:

    void syncronizationComplete();
    void newTagInsertionComplete(int tagsCount);

  public slots:

};
// UPDATETHREAD_H
#endif
#endif
