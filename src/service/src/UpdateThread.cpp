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

#include "servicelogger.h"
#include <QDebug>
#include <QSettings>
#include "UpdateThread.h"
#include "SettingsStorage.h"
#include "defines.h"
#include "PerformanceCounter.h"
#include "QueryExecutor.h"
#include "Geo2tagDatabase.h"
#if 0

UpdateThread::UpdateThread(const QList<Tag> &tags,
const QList<common::BasicUser> &users,
const QList<Channel> &channels,
const QSharedPointer<DataChannels>& dataChannelsMap,
const QSharedPointer<Sessions>& sessions,
QObject *parent)
: QThread(parent),
m_channelsContainer(channels),
m_tagsContainer(tags),
m_usersContainer(users),
m_dataChannelsMap(dataChannelsMap),
m_sessionsContainer(sessions),
m_queryExecutor(0),
m_transactionCount(0)
{
}


UpdateThread::UpdateThread(const QList<Tag> &tags,
const QList<common::BasicUser> &users,
const QList<Channel> &channels,
const QSharedPointer<DataChannels>& dataChannelsMap,
const QSharedPointer<Sessions>& sessions,
QueryExecutor* queryExecutor,
QObject *parent)
: QThread(parent),
m_channelsContainer(channels),
m_tagsContainer(tags),
m_usersContainer(users),
m_dataChannelsMap(dataChannelsMap),
m_sessionsContainer(sessions),
m_queryExecutor(queryExecutor),
m_transactionCount(0)
{
}


void UpdateThread::setQueryExecutor(QueryExecutor *queryExecutor)
{
  m_queryExecutor = queryExecutor;
}


QSharedPointer<Sessions> UpdateThread::getSessionsContainer() const
{
  return m_sessionsContainer;
}


void UpdateThread::incrementTransactionCount(int i)
{
  m_transactionCount+=i;
  DEBUG() <<  "Number of write requests: logged " << m_transactionCount;
}

QReadWriteLock *UpdateThread::getLock()
{
    return &m_updateLock;
}


bool UpdateThread::compareTransactionNumber(qlonglong factCount)
{
  bool result;
  DEBUG() << "Checking number of write requests: logged/fact" << m_transactionCount <<"/" << factCount;
  // If m_transactionCount < transactionCount then need sync
  qlonglong transactionDiff =  SettingsStorage::getValue("general/transaction_diff", QVariant(DEFAULT_TRANSACTION_DIFF_TO_SYNC)).toLongLong();
  DEBUG() << "Diff from config/fact" << transactionDiff<<"/"<< factCount - m_transactionCount;
  result = (factCount - m_transactionCount >= transactionDiff);
  if (result) m_transactionCount = factCount;

  return result;
}


void UpdateThread::run()
{
#if 0 //GT-765
  for(;;)
  {
    qlonglong interval = SettingsStorage::getValue("general/db_update_interval", QVariant(DEFAULT_DB_UPDATE_INTERVAL)).toLongLong();
    {
      PerformanceCounter counter("db_update");

      // Check if DB contain new changes
      qlonglong oldTagsContainerSize = m_tagsContainer->size();
      qlonglong factCount = QueryExecutor::instance()->getFactTransactionNumber();
      if (!compareTransactionNumber(factCount))
      {
        QThread::msleep(interval);
        continue;
      }

      QueryExecutor::instance()->checkTmpUsers();
      QueryExecutor::instance()->checkSessions(this);

      QList<common::BasicUser>       usersContainer(*m_usersContainer);
      DataMarks   tagsContainer(*m_tagsContainer);
      Channels    channelsContainer(*m_channelsContainer);
      Sessions    sessionsContainer(*m_sessionsContainer);

      QueryExecutor::instance()->loadUsers(usersContainer);
      QueryExecutor::instance()->loadChannels(channelsContainer);
      QueryExecutor::instance()->loadTags(tagsContainer);
      QueryExecutor::instance()->loadSessions(sessionsContainer);

      QWriteLocker(getLock());
      DEBUG() << "Containers locked for db_update";

      m_usersContainer->merge(usersContainer);
      m_tagsContainer->merge(tagsContainer);
      m_channelsContainer->merge(channelsContainer);
      m_sessionsContainer->merge(sessionsContainer);

      QueryExecutor::instance()->updateReflections(*m_tagsContainer,*m_usersContainer, *m_channelsContainer, *m_sessionsContainer);

      DEBUG() <<  "tags added. trying to unlock";

      if (oldTagsContainerSize != m_tagsContainer->size())
      {
        DEBUG() << "lock: filling m_dataChannelsMap ";
        for(int i=0; i<m_tagsContainer->size(); i++)
        {
          if(!m_dataChannelsMap->contains(m_tagsContainer->at(i)->getChannel(), m_tagsContainer->at(i)))
          {
            QSharedPointer<DataMark> tag = m_tagsContainer->at(i);
            Channel channel = tag->getChannel();

            DEBUG() << "adding " << i << " from "<< m_tagsContainer->size() <<" to channel " << channel->getName();
            QWriteLocker(getLock());
            m_dataChannelsMap->insert(channel, tag);
          }
        }
        Q_EMIT newTagInsertionComplete(m_tagsContainer->size()-oldTagsContainerSize);
      }
      DEBUG() <<  "current users' size = %d"     << m_usersContainer->size();
      DEBUG() <<  "current tags' size = %d"      << m_tagsContainer->size();
      DEBUG() <<  "current channels' size = %d"  << m_channelsContainer->size();
      DEBUG() <<  "current sessions' size = %d"  << m_sessionsContainer->size();

      QueryExecutor::instance()->disconnect();
      DEBUG() << "sync completed!!!";
      Q_EMIT syncronizationComplete();
    }
    QThread::msleep(interval);
  }
#endif
}
#endif
