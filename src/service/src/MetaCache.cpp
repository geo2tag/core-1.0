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
#include "SettingsStorage.h"

namespace Core
{

QMap<QString, MetaCache*> MetaCache::s_cachesMap;

MetaCache* MetaCache::getDefaultMetaCache()
{
    QString defaultDbName =
            SettingsStorage::getValue("database/name",DEFAULT_DB_NAME).toString();
    return getMetaCache(defaultDbName);
}

MetaCache* MetaCache::getMetaCache(const Session& session)
{
    DEBUG() << "Getting MetaCache for DB = " << session.getDbName();
    return getMetaCache(session.getDbName());
}

MetaCache* MetaCache::getMetaCache(const QString& dbName)
{
    MetaCache* newCache;
    if (!s_cachesMap.contains(dbName))
    {
        newCache = new MetaCache(dbName);
        s_cachesMap.insert(dbName, newCache);

    }else{
        newCache = s_cachesMap.value(dbName);
    }


    return newCache;
}


MetaCache::MetaCache(const QString& dbName):
    m_queryExecutor(QueryExecutor::getInstance(dbName)){
    DEBUG() << "Creating MetaCache for DB = " << dbName;
}


void MetaCache::init()
{
    DEBUG() << "Initializing MetaCache objects";
    //initUsers();
    //initChannels();
}

BasicUser MetaCache::getUserById(const QString userId)
{
    QReadLocker lock(&m_usersLock);

    common::BasicUser realUser;      // Null pointer

    BasicUser user;
    foreach(user,m_users)
    {
        if(QString::compare(user.getEmail(), userId, Qt::CaseInsensitive) == 0)
            return user;
    }

    return BasicUser();
}

QList<BasicUser> MetaCache::getUsers()
{
    QReadLocker lock(&m_usersLock);
    return   m_users;
}

QList<Channel> MetaCache::getChannels()
{

    return   m_queryExecutor->loadChannels();
}

bool MetaCache::addChannel(const Channel &channel, const common::BasicUser& user)
{
    bool result = m_queryExecutor->insertNewChannel(channel, user);
    
//    if (result){
//      DEBUG() << "Adding to memory channel " << channel.getName();
//      s_channels.push_back(Channel(channel));
//    }
    return result;
}

bool MetaCache::addUser(const common::BasicUser &user)
{
    bool result = m_queryExecutor->insertNewUser(user);
//    if (result)
//    {
//        QWriteLocker lock(&s_usersLock);
//	s_users.push_back(user);
//    }
    return result;
}

bool MetaCache::deleteUser(const BasicUser &user)
{
    bool result = m_queryExecutor->deleteUser(user);

//    if (result)
//    {
//      BasicUser u;
//      foreach (u, s_users)
//      {
//        if (u.getLogin() == user.getLogin())
//	{
//          QWriteLocker lock(&s_usersLock);
//          s_users.removeOne(u);
//	  break;
//        }

//      }
//    }

    return result;
}

void MetaCache::insertSession(const Session& session)
{
    m_sessions << session;
    DEBUG() << "inserted " << session << ", session count=" << m_sessions.size();
}

Channel MetaCache::getChannel(const QString name)
{
    return m_queryExecutor->getChannel(name);
}

QList<Channel> MetaCache::getChannelsByOwner(const BasicUser &user)
{
    return m_queryExecutor->getChannelsByOwner(user);
}

Session MetaCache::findSession(const BasicUser &user)
{
    QReadLocker lock(&m_SessionsLock);

    Session s;
    foreach(s,m_sessions)
    {
        if(s.getUser() == user )
        {
	    if (!s.isExpired())
	    {
              return s;
            }
	    else{
	      removeSession(s);
              continue;
            }
        }

    }
    return Session();
}

void MetaCache::removeSession(const Session& session){
  m_sessions.removeOne(session);
}


Session MetaCache::findSession(const QString &token)
{
    DEBUG() << "Looking op for a session, token " << token;
    Session s;
    foreach(s,m_sessions)
    {
        DEBUG() << "test:" << s;
        if(s.getSessionToken() == token )
        {
	    if (!s.isExpired())
	    {
              DEBUG() << "...found " << s;
              return s;
            }
	    else{
	      removeSession(s);
              break;
            }

        }
    }
    DEBUG() << "...not found ";
    return Session();
}

Channel MetaCache::findChannel(const QString &name)
{
//#ifdef GEO2TAG_LITE
    return m_queryExecutor->getChannel(name);
//#else
//    Channel ch;
//    QReadLocker lock(&s_channelsLock);
//    foreach(ch,s_channels)
//    {
//	DEBUG() << "Checking channel " << ch.getName();
//        if(QString::compare(ch.getName(), name, Qt::CaseInsensitive) == 0)
//            return ch;
//    }
//    return Channel();
//#endif
}

QList<Channel> MetaCache::getSubscribedChannels(const BasicUser &user)
{
    return m_queryExecutor->getSubscribedChannels(user);
}

bool MetaCache::subscribeChannel(const BasicUser &user, const Channel &channel)
{
    return m_queryExecutor->subscribeChannel(user,channel);
}

bool MetaCache::unsubscribeChannel(const BasicUser &user, const Channel &channel)
{
    return m_queryExecutor->unsubscribeChannel(user,channel);
}

void MetaCache::reloadSessions()
{
    QWriteLocker lock(&m_SessionsLock);
    initSessions();
}

bool MetaCache::checkEmail(const QString& email){

  bool result = m_queryExecutor->checkEmail(email);
  return result;

}

bool MetaCache::checkUser(BasicUser &user)
{

    BasicUser bu = m_queryExecutor->getUser(user.getLogin());


    DEBUG() << "Checking " << user << " ->" << bu.isValid();

    return bu.isValid();
}

bool MetaCache::testChannel(BasicUser &user, const Channel& channel)
{
    DEBUG() << "check channel " << channel.getName() << " for " << user.getLogin();

    if(!channel.isValid())
    {
        WARNING() << "Channel " << channel << " is invalid";
        return false;
    }

#ifdef GEO2TAG_LITE
    return true;
#else
    return m_queryExecutor->isSubscribed(user,channel);
#endif
}

bool MetaCache::writeTag(const Tag &tag)
{
    return m_queryExecutor->insertNewTag(tag);
}

QList<Tag> MetaCache::loadTagsFromChannel(const Channel &channel)
{
    return m_queryExecutor->loadTags(channel);
}

void MetaCache::initUsers()
{
    QWriteLocker lock(&m_cacheLock);

    DEBUG() << "Initializing Users";
    m_users = m_queryExecutor->loadUsers();
    DEBUG() << "Loaded " << m_users.size() << "users";
    BasicUser u;
    foreach(u, m_users)
    {
       DEBUG() << u;
    }
}

BasicUser MetaCache::findUserByName(const QString& name){
//    BasicUser u;
 
//    foreach(u,s_users)
//    {
//        if(QString::compare(u.getLogin(), name, Qt::CaseInsensitive) == 0 )
//            return u;
//    }

//    return BasicUser();
    return m_queryExecutor->getUser(name);
}


bool MetaCache::alterChannel(const QString& name, const QString& field, const QString& value){
	return m_queryExecutor->alterChannel(name, field, value);
}

void MetaCache::updateSession(Session &session)
{
    QDateTime currentTime = QDateTime::currentDateTime();
    session.setLastAccessTime(currentTime);
}

void MetaCache::initSessions()
{
#if GEO2TAG_LITE
    DEBUG() << "Session persistans is not supported in lite version";
#else
    //NOT_IMPLEMENTED();
#endif
}

void MetaCache::initChannels()
{
    QWriteLocker lockf(&m_channelsLock);
#ifdef GEO2TAG_LITE
    // initialization is not supported;
#else
    QWriteLocker lock(&m_cacheLock);


    DEBUG() << "Initializing Channels";
    m_channels = m_queryExecutor->loadChannels();
    DEBUG() << "Loaded " << m_users.size() << "channels";
#endif
}

void MetaCache::changeDbName(const Session& session, const QString& dbName)
{
    QWriteLocker lock(&m_SessionsLock);
    for(int i=0; i<m_sessions.size(); i++){
        if (m_sessions[i] == session){
            DEBUG() << "Found Session";



            m_sessions[i].setDbName(dbName);

            DEBUG() << "After changes " <<
                       getDefaultMetaCache()->findSession(session.getSessionToken()).getDbName();
            return;
        }
    }
    DEBUG() << "Session not found for changing dbName";
}

bool MetaCache::isOwner(const Session& session, const QString& channel){
	return m_queryExecutor->isOwner(session.getUser().getLogin(), channel);
}

bool MetaCache::changePassword(const QString& login, const QString& newPassword){
	return m_queryExecutor->changePassword(login, newPassword);
}

bool MetaCache::isDefaultMetaCache() const {
	return this == MetaCache::getDefaultMetaCache();
}

void MetaCache::touchUserToServiceDb(const common::BasicUser& user){

	if (isDefaultMetaCache()) return;

	common::BasicUser serviceUser = m_queryExecutor->getUser(user.getLogin());	
	DEBUG() << serviceUser;
	if (serviceUser.isValid()) return;

	m_queryExecutor->insertNewUser(user);

}


QList<Tag> MetaCache::loadTagsWithSubstring(const QString& field, const QString& substring,
	 const Channel &channel){
	return m_queryExecutor->loadTagsWithSubstring(field, substring, channel);
}

} // namespace Core
