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
#include <QCryptographicHash>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QString>

#include "defines.h"
#include "QueryExecutor.h"
#include "JsonUser.h"
#include "DataMarkInternal.h"
#include "UserInternal.h"
#include "ChannelInternal.h"
#include "SessionInternal.h"
#include "SettingsStorage.h"
#include "PerformanceCounter.h"
#include "EmailMessage.h"

#include "MetaCache.h"

QMap<QString, QueryExecutor*> QueryExecutor::s_executorsMap;


qlonglong QueryExecutor::nextKey(const QString &sequence) const
{
    QSqlQuery query=makeQuery();
    query.exec("select nextval('"+sequence+"') as seq;");
    qlonglong id=-1;
    if ( query.next() )
        id = query.value(0).toLongLong();
    return id;
}

QueryExecutor::QueryExecutor(const QString &dbName): m_dbName(dbName)
{
    DEBUG() << "Calling QueryExecutor constructor for " << m_dbName;
    initDatabase(dbName);
}


qlonglong QueryExecutor::nextTagKey() const
{
    return nextKey("tags_seq");
}


qlonglong QueryExecutor::nextUserKey() const
{
    return nextKey("users_seq");
}


qlonglong QueryExecutor::nextChannelKey() const
{
    return nextKey("channels_seq");
}


qlonglong QueryExecutor::nextSessionKey() const
{
    return nextKey("sessions_seq");
}


const QString QueryExecutor::generateNewToken(const QString& accessTime, const QString& email, const QString& login,const QString& password) const
{
    QString log=login+password+email+accessTime;
    QByteArray toHash(log.toUtf8());
    toHash=QCryptographicHash::hash(log.toUtf8(),QCryptographicHash::Md5);
    QString result(toHash.toHex());
    DEBUG() << "TOken = %s" << result;
    return result;
}


bool QueryExecutor::insertNewTag(const Tag &tag)
{
    PerformanceCounter counter("QueryExecutor::insertNewTag");
    bool result;

    qlonglong newId = nextTagKey();

    DEBUG() <<  QString("inserting tag ") << tag << ", id=" << newId;

    QSqlQuery newTagQuery=makeQuery();
    newTagQuery.prepare("insert into tag (altitude , latitude, longitude, label, description, url, user_id, time, id, channel_id) "
                        "         values(:altitude,:latitude,:longitude,:label,:description,:url,:user_id,:time,:id, :channel_id);");
    newTagQuery.bindValue(":altitude", tag.getAltitude());
    newTagQuery.bindValue(":latitude", tag.getLatitude());
    newTagQuery.bindValue(":longitude", tag.getLongitude());
    newTagQuery.bindValue(":label", tag.getLabel().isEmpty() ? "unnamed" : tag.getLabel());
    newTagQuery.bindValue(":description", tag.getDescription());
    newTagQuery.bindValue(":url", tag.getUrl());


    qlonglong userId = getUserIdByName(tag.getUser().getLogin());
    qlonglong channelId = getChannelIdByName(tag.getChannel().getName());


    newTagQuery.bindValue(":user_id", userId);
    newTagQuery.bindValue(":channel_id", channelId);
    newTagQuery.bindValue(":time", tag.getTime()/*.toUTC()*/);
    newTagQuery.bindValue(":id", newId);

    transaction();

    result = newTagQuery.exec();
    if(!result)
    {
        rollback();
        DEBUG() <<  "Rollback for NewTag sql query";
        return false;
    }

    commit();

    return true;
}


bool QueryExecutor::insertNewChannel(const Channel &channel, const common::BasicUser& user)
{
    PerformanceCounter counter("QueryExecutor::insertNewChannel");

    bool result;
    QSqlQuery newChannelQuery=makeQuery();
    qlonglong newId = nextChannelKey();
    DEBUG() << "NewId ready, now preparing sql query for adding new channel";
    newChannelQuery.prepare("insert into channel (id,name,description,url,owner_id) values(:id,:name,:description,:url,:owner_id);");
    newChannelQuery.bindValue(":id",newId);
    newChannelQuery.bindValue(":name",channel.getName());
    newChannelQuery.bindValue(":description",channel.getDescription());
    newChannelQuery.bindValue(":url",channel.getUrl());

    qlonglong owner = getUserIdByName(user.getLogin());
    newChannelQuery.bindValue(":owner_id",owner);

    DEBUG() << "Writing channel " << channel.getName() << " for user " << user << "(" << owner << ")";


    transaction();

    result=newChannelQuery.exec();
    if(!result)
    {
        DEBUG() << "Rollback for NewChannel sql query";
        rollback();
    }else{
        DEBUG() << "Commit for NewChannel sql query - insert in table channel";
        commit();
    }
    //QSharedPointer<DbChannel> newChannel(new DbChannel(newId,channel.getName(),channel.getDescription(),channel.getUrl(),channel.getOwner()));
    return result;
}

bool QueryExecutor::doesUserWithGivenEmailExist(const common::BasicUser &user)
{
    PerformanceCounter counter("QueryExecutor::doesUserWithGivenEmailExist");
    QSqlQuery query=makeQuery();
    DEBUG() <<  "Checking of user existence in users by email: "<<user.getEmail();

    query.prepare("select id from users where email = :email;");
    query.bindValue(":email", user.getEmail());
    DEBUG() << "Selecting: " <<  query.lastQuery();
    query.exec();

    if (query.next())
    {
        DEBUG() << "Match found.";
        return true;
    }
    else
    {
        DEBUG() << "No matching users.";
        return false;
    }
}

bool QueryExecutor::doesRegistrationTokenExist(const QString &token)
{
    PerformanceCounter counter("QueryExecutor::doesRegistrationTokenExist");
    QSqlQuery query=makeQuery();
    DEBUG() <<  "Checking of user existence in signups by token:" <<  token.toStdString().c_str();

    query.prepare("select id from signups where registration_token = :token;");
    query.bindValue(":token", token);
    DEBUG() << "Selecting: "<< query.lastQuery().toStdString().c_str();
    query.exec();

    if (query.next())
    {
        DEBUG() << "Match found.";
        return true;
    }
    else
    {
        DEBUG() << "No matching users.";
        return false;
    }
}


bool QueryExecutor::insertNewUser(const common::BasicUser& user)
{
    PerformanceCounter counter("QueryExecutor::insertNewUser");

    QSqlQuery newUserQuery=makeQuery();

    DEBUG() << "inserting user " << user;

//   QString qry="insert into users (email,login,password) values('%1','%2','%3');";
//    qry=qry.arg(user.getEmail()).arg(user.getLogin()).arg(user.getPassword());
//    DEBUG() << "stmt=" << qry;
    newUserQuery.prepare("insert into users (email,login,password) values(:email, :login, :password);");
    newUserQuery.bindValue(":email", user.getEmail());
    newUserQuery.bindValue(":login", user.getLogin());
    newUserQuery.bindValue(":password", user.getPassword());

    transaction();
    bool result = newUserQuery.exec();

    if  (result ){

	commit();
	DEBUG() << "insertNewUser commit";
	return true;	
    }else {

	rollback();
	DEBUG() << "insertNewUser rollback";
	return false;	
	
    }	


}


bool QueryExecutor::subscribeChannel(const common::BasicUser& user,const Channel& channel)
{
    PerformanceCounter counter("QueryExecutor::subscribeChannel");
    bool result;
    QSqlQuery insertNewSubscribtion=makeQuery();

    qlonglong userId = getUserIdByName(user.getLogin());
    qlonglong channelId = getChannelIdByName(channel.getName());


    insertNewSubscribtion.prepare("insert into subscribe (channel_id,user_id) values(:channel_id,:user_id);");
    insertNewSubscribtion.bindValue(":channel_id",channelId);
    insertNewSubscribtion.bindValue(":user_id",userId);
    DEBUG() << "Subscribing "<<user.getLogin()<<" (Id = "<<userId
            <<") for "<<channel.getName()<<" (Id = "<<channelId <<")";

    transaction();
    result=insertNewSubscribtion.exec();
    if(!result)
    {
        DEBUG() << "Rollback for subscribeChannel sql query";
        rollback();
    }else
    {
        DEBUG() << "Commit for subscribeChannel sql query";
        commit();
    }
    return result;
}


bool QueryExecutor::unsubscribeChannel(const common::BasicUser& user,const Channel& channel)
{
    PerformanceCounter counter("QueryExecutor::unsubscribeChannel");
    bool result;
    QSqlQuery deleteSubscribtion=makeQuery();
    qlonglong userId = getUserIdByName(user.getLogin());
    qlonglong channelId = getChannelIdByName(channel.getName());

    deleteSubscribtion.prepare("delete from subscribe where channel_id = :channel_id AND user_id = :user_id;");
    deleteSubscribtion.bindValue(":channel_id",channelId);
    deleteSubscribtion.bindValue(":user_id",userId);
    DEBUG() << "Unsubscribing " << user.getLogin() << " (Id = " << userId
            << ") for " << channel.getName() << " (Id = " << channelId <<")";

    transaction();

    result=deleteSubscribtion.exec();
    if(!result)
    {
        DEBUG() << "Rollback for unsubscribeChannel sql query";
        rollback();
    }else
    {
        DEBUG() << "Commit for unsubscribeChannel sql query";
        commit();
    }
    return result;
}


bool QueryExecutor::deleteUser(const common::BasicUser &user)
{
    PerformanceCounter counter("QueryExecutor::deleteUser");
    bool result;
    QSqlQuery deleteUserQuery=makeQuery();
    qlonglong userId = getUserIdByName(user.getLogin());

    DEBUG() << "Deleting: id = " << userId;
    deleteUserQuery.prepare("delete from users where id = :id;");
    deleteUserQuery.bindValue(":id",userId);

    transaction();

    result = deleteUserQuery.exec();
    if(!result)
    {
        DEBUG() << "Rollback for deleteUser sql query";
        rollback();
    }
    else
    {
        DEBUG() << "Commit for deleteUser sql query";
        commit();
    }
    return result;
}


common::BasicUser QueryExecutor::updateUserPassword(common::BasicUser& user, const QString& password)
{
    QSqlQuery query=makeQuery();
    qlonglong userId = getUserIdByName(user.getLogin());

    DEBUG() <<  "Updating password for user with id:" << userId;
    query.prepare("update users set password = :pwd where id = :id;");
    query.bindValue(":pwd", password);
    query.bindValue(":id", userId);

    transaction();

    bool result = query.exec();
    if (!result)
    {
        DEBUG() << "Rollback for updateUserPassword sql query";
        rollback();
        return common::BasicUser(NULL);
    }
    else
    {
        DEBUG() << "Commit for updateUsersPassword sql query";
        commit();
        user.setPassword(password);
        return user;
    }
}


bool QueryExecutor::insertNewSession(const Session& session)
{
    QSqlQuery query=makeQuery();
    qlonglong newId = nextSessionKey();

    DEBUG() <<  "NewId ready, now preparing sql query for adding new session";
    query.prepare("insert into sessions (id, user_id, session_token, last_access_time) values (:id, :user_id, :token, :time);");
    query.bindValue(":id", newId);
    query.bindValue(":user_id", QueryExecutor::getUserIdByName(session.getUser().getLogin()));
    query.bindValue(":token", session.getSessionToken());
    query.bindValue(":time", session.getLastAccessTime());

    transaction();

    bool result = query.exec();
    if (!result)
    {
        DEBUG() << "Rollback for NewSession sql query";
        rollback();
        return false;
    }
    else
    {
        DEBUG() << "Commit for NewSession sql query - insert in table sessions";
        commit();
    }
    return true;
}


bool QueryExecutor::updateSession(Session &session)
{
    QSqlQuery query=makeQuery();
    QDateTime currentTime = QDateTime::currentDateTime();
    DEBUG() <<  "Updating session with token: " << session.getSessionToken();

    query.prepare("update sessions set last_access_time = :time where session_token = :token;");
    query.bindValue(":time", currentTime);
    query.bindValue(":token", session.getSessionToken());

    transaction();

    bool result = query.exec();
    if (!result)
    {
        DEBUG() << "Rollback for updateSession sql query";
        rollback();
        return false;
    }
    else
    {
        DEBUG() << "Commit for updateSession sql query";
        commit();
        session.setLastAccessTime(currentTime);
        return true;
    }
}


bool QueryExecutor::deleteSession(const Session &session)
{
    QSqlQuery query=makeQuery();
    DEBUG() <<  "Deleting session with token: " <<  session.getSessionToken();

    query.prepare("delete from sessions where session_token = :id;");
    query.bindValue(":id", session.getSessionToken());

    transaction();

    bool result = query.exec();
    if (!result)
    {
        DEBUG() <<  "Rollback for deleteSession sql query";
        rollback();
        return false;
    }
    else
    {
        DEBUG() <<  "Commit for deleteSession sql query";
        commit();
        return true;
    }
}



QList<common::BasicUser> QueryExecutor::loadUsers()
{
    QList<common::BasicUser> result;
    QSqlQuery query=makeQuery();
    query.exec("select id, login, password, email from users order by id;");
    while (query.next())
    {
        QString login = query.record().value("login").toString();
        QString password = query.record().value("password").toString();
        QString email = query.record().value("email").toString();
        result.push_back(common::BasicUser(login,password,email));
    }
    return result;
}


bool QueryExecutor::checkEmail(const QString& email)
{

    QSqlQuery query=makeQuery();
    //QString qry("select email from users where lower(email) = lower('%1');");
    query.prepare("select email from users where lower(email) = lower(:email);");
    query.bindValue(":email", email);

    DEBUG() << "QueryExecutor::checkEmail " << email;

    query.exec();
    if (query.next())
    {
	return true;
    }
    return false;

}

common::BasicUser QueryExecutor::getUser(const QString &login)
{
    QSqlQuery query=makeQuery();
    query.prepare("select id, login, password, email from users where lower(login) = lower(:login);");
    query.bindValue(":login", login);

    DEBUG() << "QueryExecutor::getUser " << login;

    query.exec();
    if (query.next())
    {
        QString login = query.record().value("login").toString();
        QString password = query.record().value("password").toString();
        QString email = query.record().value("email").toString();
        return common::BasicUser(login,password,email);
    }
    return common::BasicUser();
}

common::BasicUser QueryExecutor::getUserById(qlonglong id)
{
    QSqlQuery query=makeQuery();
    query.prepare("select login, password, email from users where id=:Id ;");
    query.bindValue(":Id", id);


    query.exec();
    if (query.next())
    {
        QString login = query.record().value("login").toString();
        QString password = query.record().value("password").toString();
        QString email = query.record().value("email").toString();
        return common::BasicUser(login,password,email);
    }
    return common::BasicUser();
}

QList<Channel> QueryExecutor::getChannelsByOwner(const common::BasicUser &user)
{
    QList<Channel> container;

    QSqlQuery query=makeQuery();
    qlonglong owner_id = getUserIdByName(user.getLogin());
    query.prepare("select description, name, url from channel where owner_id=:ownerId;");
    query.bindValue(":ownerId", owner_id);

    query.exec();

    DEBUG() << "Found " << query.size() << " channels";
    while (query.next())
    {
        QString name = query.record().value("name").toString();
        QString description = query.record().value("description").toString();
        QString url = query.record().value("url").toString();
        qlonglong ownerId = query.record().value("owner_id").toLongLong();

        QSqlQuery selectQuery=makeQuery();
        selectQuery.prepare("select email, login, password from users where id = :owner_id;");
        selectQuery.bindValue(":owner_id", ownerId);
        selectQuery.exec();
        selectQuery.next();

        QString email = selectQuery.record().value("email").toString();
        QString login = selectQuery.record().value("login").toString();
        QString passw = selectQuery.record().value("password").toString();

        Channel channel(name,description,url);
        container.push_back(channel);
    }

    return container;
}

QList<Channel> QueryExecutor::getSubscribedChannels(const common::BasicUser &user)
{
    QSqlQuery query=makeQuery();
    QList<Channel> container;
    qlonglong userId = getUserIdByName(user.getLogin());

    query.prepare("select channel.name, channel.id, channel.description, channel.url from channel inner join subscribe on channel.id = subscribe.channel_id inner join users on subscribe.user_id = users.id where users.id=:userId;");
    query.bindValue(":userId", userId);

    query.exec();

    DEBUG() << "User " << user.getLogin() << " subscribed to " << query.size() << " channels";

    while (query.next())
    {
        QString name = query.record().value("name").toString();
        QString description = query.record().value("description").toString();
        QString url = query.record().value("url").toString();

        Channel channel(name,description,url);
        container.push_back(channel);
    }
    return container;
}


QList<Channel> QueryExecutor::loadChannels()
{
    QSqlQuery query=makeQuery();
    QList<Channel> container;
    query.exec("select id, description, name, url, owner_id from channel order by id;");
    while (query.next())
    {
        QString name = query.record().value("name").toString();
        QString description = query.record().value("description").toString();
        QString url = query.record().value("url").toString();
        qlonglong ownerId = query.record().value("owner_id").toLongLong();

        QSqlQuery selectQuery=makeQuery();
        selectQuery.prepare("select email, login, password from users where id = :owner_id;");
        selectQuery.bindValue(":owner_id", ownerId);
        selectQuery.exec();
        selectQuery.next();

        QString email = selectQuery.record().value("email").toString();
        QString login = selectQuery.record().value("login").toString();
        QString passw = selectQuery.record().value("password").toString();

        Channel channel(name,description,url);
        container.push_back(channel);
    }
    return container;
}


QList<Tag> QueryExecutor::loadTags()
{
    QList<Tag> container;
    QSqlQuery query=makeQuery();
    query.exec("select time, altitude, latitude, longitude, label, description, url, user_id, channel_id from tag order by time;");
    while (query.next())
    {
        QDateTime time = query.record().value("time").toDateTime();//.toTimeSpec(Qt::LocalTime);
        qreal latitude = query.record().value("latitude").toReal();
        qreal altitude = query.record().value("altitude").toReal();
        qreal longitude = query.record().value("longitude").toReal();
        QString label = query.record().value("label").toString();
        QString description = query.record().value("description").toString();
        QString url = query.record().value("url").toString();

	qlonglong userId = query.record().value("user_id").toLongLong();
	common::BasicUser user = getUserById(userId);
        qlonglong channelId = query.record().value("channel_id").toLongLong();
	Channel channel = getChannelById(channelId); 

        Tag tag(altitude,latitude,longitude,label,description,url,time);
	tag.setUser(user);
	tag.setChannel(channel);
        container.push_back(tag);
    }
    return container;
}

QList<Tag> QueryExecutor::loadTags(const Channel &channel)
{
    QList<Tag> container;
    qlonglong channelId=getChannelIdByName(channel.getName());

    DEBUG() << "Loading tags for " << channel.getName();

    QSqlQuery query=makeQuery();
    query.prepare("select time, altitude, latitude, longitude, label, description, url, user_id, channel_id "
                       "from tag where channel_id = :channelId order by time;");
    query.bindValue(":channelId", channelId);

    query.exec();
    while (query.next())
    {
        QDateTime time = query.record().value("time").toDateTime().toTimeSpec(Qt::LocalTime);
        qreal latitude = query.record().value("latitude").toReal();
        qreal altitude = query.record().value("altitude").toReal();
        qreal longitude = query.record().value("longitude").toReal();
        QString label = query.record().value("label").toString();
        QString description = query.record().value("description").toString();
        QString url = query.record().value("url").toString();

        qlonglong userId = query.record().value("user_id").toLongLong();
	common::BasicUser user = getUserById(userId);
        qlonglong channelId = query.record().value("channel_id").toLongLong();
	Channel channel = getChannelById(channelId); 

        Tag tag(altitude,latitude,longitude,label,description,url,time);
	tag.setUser(user);
	tag.setChannel(channel);

        container.push_back(tag);
    }
    DEBUG() << ".... done, amount = " << container.size();
    return container;
}

Channel QueryExecutor::getChannelById(qlonglong id)
{
    QSqlQuery query=makeQuery();
    query.prepare("select name, description, url from channel where id=:channelId;");
    query.bindValue(":channelId", id);


    query.exec();
    if (query.next())
    {
        QString name = query.record().value("name").toString();
        QString description = query.record().value("description").toString();
        QString url = query.record().value("url").toString();
        return Channel(name, description, url);
    }
    return Channel();

}

QList<Session> QueryExecutor::loadSessions()
{
    // NOTE: Users should be loaded before!

    QList<Session> result;
    QSqlQuery query=makeQuery();

    query.exec("select u.login as login, u.password as password, u.email as email, s.session_token, s.last_access_time "\
               "from users as u, sessions as s where u.id=s.user_id;");

    while (query.next())
    {
        QString   login     = query.record().value("login").toString();
        QString   password  = query.record().value("password").toString();
        QString   email     = query.record().value("email").toString();
        QString   token = query.record().value("session_token").toString();
        QDateTime lastAccessTime = query.record().value("last_access_time").toDateTime();

        common::BasicUser user(login,password,email);
        if(Core::MetaCache::getDefaultMetaCache()->checkUser(user))
        {
            Session session(token,lastAccessTime,user);
            result.push_back(session);
            DEBUG() << "added session " << session.getSessionToken() << " for " << user;
        } else
        {
            DEBUG() << "User " << user << " is not valid";
        }
    }
    return result;
}


qlonglong QueryExecutor::getUserIdByName(const QString &name)
{
    QSqlQuery query=makeQuery();
    DEBUG() << "getUserIdByName " << name;

    query.prepare("select id from users where lower(login)=lower(:login);");
    query.bindValue(":login", name);
    query.exec();
    qlonglong id =1;  //default value

    if(!query.next())
    {
        qCritical() << "Cannot find existing user " << name;
    }
    else
    {
        id = query.record().value("id").toLongLong();
    }

    return id;
}

qlonglong QueryExecutor::getChannelIdByName(const QString &name)
{
    QSqlQuery query=makeQuery();
    query.prepare("select id from channel where lower(name) = lower(:name);");
    query.bindValue(":name", name);
    query.exec();

    qlonglong id =-1;

    if(!query.next())
    {
        CRITICAL() << "Cannot find existing channel " << name;
    }
    else
    {
        id = query.record().value("id").toLongLong();
    }

    return id;
}

Channel QueryExecutor::getChannel(const QString &name)
{
    QSqlQuery query=makeQuery();

    DEBUG() << "Looking up channel " << name;
    //QString q=QString("select name, description, url from channel where lower(name) = lower('%1');").arg(name);
    query.prepare("select name, description, url from channel where lower(name) = lower(:name);");
    query.bindValue(":name", name);

    DEBUG() << "Query = " << query.lastQuery();

    query.exec();

    DEBUG() << " ... found " << query.size() << "results, take first if possibe";


    if(!query.next())
    {
        WARNING() << "Cannot find channel \"" << name << "\"";
        return Channel();
    }
    else
    {
        QString name = query.record().value("name").toString();
        QString description = query.record().value("description").toString();
        QString url = query.record().value("url").toString();
        return Channel(name,description,url);
    }
}

bool QueryExecutor::isSubscribed(const common::BasicUser &user, const Channel &channel)
{
    DEBUG() << "Check subscribtion " << user.getLogin() << " to " << channel;

    QSqlQuery query=makeQuery();

    qlonglong userId = getUserIdByName(user.getLogin());
    qlonglong channelId = getChannelIdByName(channel.getName());

    query.prepare("select * from subscribe where channel_id=:chanelId and user_id=:userId;");
    query.bindValue(":chanelId", channelId);
    query.bindValue(":userId", userId);
    

    query.exec();


    if(query.size()>0)
    {
        DEBUG() << "... -> SUBSCRIBED";
        return true;
    }
    else
    {
        DEBUG() << "... -> NOT SUBSCRIBED";
        return false;
    }
}


qlonglong QueryExecutor::getFactTransactionNumber()
{
    NOT_IMPLEMENTED();

//    QSqlQuery query=makeQuery();

//    query.exec("select tup_inserted ,tup_updated ,tup_deleted from  pg_stat_database where datname='geo2tag';");
//    query.next();
//    qlonglong factCount = query.record().value("tup_inserted").toLongLong() +
//            query.record().value("tup_updated").toLongLong() +
//            query.record().value("tup_deleted").toLongLong();

//    return factCount;
    return 0;
}

QSqlQuery QueryExecutor::makeQuery() const
{
    return QSqlQuery(QSqlDatabase::database(m_dbName));
}

void QueryExecutor::transaction()
{
    QSqlDatabase::database().transaction();
}

void QueryExecutor::rollback()
{
    QSqlDatabase::database().rollback();
}

void QueryExecutor::commit()
{
    QSqlDatabase::database().commit();
}

QueryExecutor *QueryExecutor::getInstance(const QString &dbName)
{
    QueryExecutor* result;
    if (!s_executorsMap.contains(dbName)){
        result = new QueryExecutor(dbName);
        s_executorsMap.insert(dbName, result);
    }else{
        result = s_executorsMap.value(dbName);
    }
    return result;
}


bool QueryExecutor::checkDbExistance(const QString & dbName)
{

    bool connectionOpeningResult = initDatabase("postgres");
    if (!connectionOpeningResult)
    {
      DEBUG() << "Unable to connect to postgres!";
      return false;
    }


    QSqlQuery getDbNameQuery = QSqlQuery(QSqlDatabase::database("postgres"));
    getDbNameQuery.prepare("select datname from pg_database where lower(datname) = lower(:dbName);");
    getDbNameQuery.bindValue(":dbName", dbName);

    getDbNameQuery.exec();

    if (!getDbNameQuery.next())
    {
      DEBUG() << "Database " << dbName << " does not exist on this server";
      return false;
    }

    DEBUG() << "Database " << dbName << " exists on this server";
    return true;
}

bool QueryExecutor::initDatabase(const QString & dbName){
    DEBUG() << "Initializing DB = " << dbName;

    //GT-817 Now is only QPSQL base is supported

    QSqlDatabase oldDatabase = QSqlDatabase::database(dbName);
    if (oldDatabase.isOpen())
    {
        DEBUG() << dbName <<" is still opened, closing";
        oldDatabase.close();
    }

    QSqlDatabase database = QSqlDatabase::addDatabase("QPSQL", dbName);

    QVariant defaultName("COULD_NOT_READ_CONFIG");
    QString host=SettingsStorage::getValue("database/host",defaultName).toString();

    QString user=SettingsStorage::getValue("database/user",defaultName).toString();
    QString pass=SettingsStorage::getValue("database/password",defaultName).toString();

    DEBUG() << host << user << pass;

    database.setHostName(host);
    database.setDatabaseName(dbName);
    database.setUserName(user);
    database.setPassword(pass);

    DEBUG() << "Connecting to " << database.databaseName() << ", options= " << database.connectOptions();
    bool result = database.open();
    DEBUG() << "database.open()=" << result;

    return result;

}

bool QueryExecutor::alterChannel(const QString& name, const QString& field, const QString& value){

    if ( !Channel::isFieldNameValid(field)) return false;

    QSqlQuery query=makeQuery();

    qlonglong channelId = getChannelIdByName(name);

    QString queryString = QString("update channel set %1=:value where id=:chanelId;").arg(field);

    query.prepare(queryString);
    query.bindValue(":chanelId", channelId);
    query.bindValue(":value", value);
    

    transaction();

    bool result = query.exec();
    if(result)
    {
    	commit();
        DEBUG() <<  "Commit for AlterChannel sql query";
    }else{
	rollback();
        DEBUG() <<  "Rollback for AlterChannel sql query";
    }


    return result;
}

bool QueryExecutor::isOwner(const QString& name, const QString& channel){

    QSqlQuery query = makeQuery();
    qlonglong userId = getUserIdByName(name);
    qlonglong channelId = getChannelIdByName(channel);

    query.prepare("select * from channel where owner_id=:userId and id=:channelId;");
    query.bindValue(":userId", userId);
    query.bindValue(":channelId", channelId);

    query.exec();

    return (query.size()>0);
}
