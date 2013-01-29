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

QueryExecutor::QueryExecutor()
{
}


qlonglong QueryExecutor::nextKey(const QString &sequence) const
{
    QSqlQuery query=makeQuery();
    query.exec("select nextval('"+sequence+"') as seq;");
    qlonglong id=-1;
    if ( query.next() )
        id = query.value(0).toLongLong();
    return id;
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

    DEBUG() <<  QString("insertNewTag-start-").append(QString::number(newId));

    QSqlQuery newTagQuery=makeQuery();
    newTagQuery.prepare("insert into tag (altitude , latitude, longitude, label, description, url, user_id, time, id, channel_id) "
                        "         values(:altitude,:latitude,:longitude,:label,:description,:url,:user_id,:time,:id, :channel_id);");
    newTagQuery.bindValue(":altitude", tag.getAltitude());
    newTagQuery.bindValue(":latitude", tag.getLatitude());
    newTagQuery.bindValue(":longitude", tag.getLongitude());
    newTagQuery.bindValue(":label", tag.getLabel().isEmpty() ? "New Mark" : tag.getLabel());
    newTagQuery.bindValue(":description", tag.getDescription());
    newTagQuery.bindValue(":url", tag.getUrl());


    qlonglong userId = QueryExecutor::instance()->getUserIdByName(tag.getUser().getLogin());
    qlonglong channelId = QueryExecutor::instance()->getChannelIdByName(tag.getChannel().getName());


    newTagQuery.bindValue(":user_id", userId);
    newTagQuery.bindValue(":channel_id", channelId);
    newTagQuery.bindValue(":time", tag.getTime().toUTC());
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


    newChannelQuery.bindValue(":owner_id",QueryExecutor::instance()->getUserIdByName(user.getLogin()));

    transaction();

    result=newChannelQuery.exec();
    if(!result)
    {
        DEBUG() << "Rollback for NewChannel sql query";
        rollback();
        return false;
    }else
        DEBUG() << "Commit for NewChannel sql query - insert in table channel";

    commit();

    //QSharedPointer<DbChannel> newChannel(new DbChannel(newId,channel.getName(),channel.getDescription(),channel.getUrl(),channel.getOwner()));
    return true;
}


bool QueryExecutor::doesTmpUserExist(const common::BasicUser &user)
{
    PerformanceCounter counter("QueryExecutor::doesTmpUserExist");
    QSqlQuery query=makeQuery();
    query.prepare("select * from signups where login = :login or email = :email;");
    query.bindValue(":login",user.getLogin());
    query.bindValue(":email",user.getEmail());
    DEBUG() << "Selecting: %s" <<  query.lastQuery();

    query.exec();

    if (query.next())
    {
        return true;
    }
    else
    {
        DEBUG() << "No matching users.";
        return false;
    }
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


bool QueryExecutor::deleteTmpUser(const common::BasicUser &user)
{
    PerformanceCounter counter("QueryExecutor::deleteTmpUser");
    bool result;
    QSqlQuery deleteSignupQuery=makeQuery();
    deleteSignupQuery.prepare("delete from signups where login = :login;");
    deleteSignupQuery.bindValue(":login",user.getLogin() );
    DEBUG() << "Deleting: "<< deleteSignupQuery.lastQuery();

    transaction();

    result = deleteSignupQuery.exec();
    if(!result)
    {
        DEBUG() << "Rollback for deleteSignup sql query";
        rollback();
    }
    else
    {
        DEBUG() << "Commit for deleteSignup sql query";
        commit();
    }
    return result;
}


bool QueryExecutor::insertNewTmpUser(const common::BasicUser &)
{
    NOT_IMPLEMENTED();
#if 0
    PerformanceCounter counter("QueryExecutor::insertNewTmpUser");
    bool result;
    QSqlQuery newSignupQuery=makeQuery();
    qlonglong newId = nextUserKey();
    DEBUG() << "Generating token for new signup " << user.getLogin() << user.getPassword();
    QString newToken = generateNewToken(user.getEmail(), user.getLogin(),user.getPassword());
    newSignupQuery.prepare("insert into signups (id,email,login,password,registration_token,sent) values(:id,:email,:login,:password,:r_token,:sent);");
    newSignupQuery.bindValue(":id", newId);
    newSignupQuery.bindValue(":email", user.getEmail());
    newSignupQuery.bindValue(":login", user.getLogin());
    newSignupQuery.bindValue(":password", user.getPassword());
    newSignupQuery.bindValue(":r_token", newToken);
    newSignupQuery.bindValue(":sent", FALSE);

    transaction();

    result = newSignupQuery.exec();
    if(!result)
    {
        DEBUG() << "Rollback for NewSignup sql query";
        rollback();
        return false;
    }
    DEBUG() << "Commit for NewSignup sql query";
    commit();

    return true;
#endif
    return false;
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


common::BasicUser QueryExecutor::insertTmpUserIntoUsers(const QString &token)
{
    PerformanceCounter counter("QueryExecutor::insertTmpUserIntoUsers");
    QSqlQuery checkQuery=makeQuery();
    DEBUG() <<  "Checking of user existence in signups by token: %s"<< token;

    checkQuery.prepare("select email, login, password from signups where registration_token = :token;");
    checkQuery.bindValue(":token", token);
    DEBUG() << "Selecting: %s"<< checkQuery.lastQuery();
    checkQuery.exec();

    if (checkQuery.next())
    {
        DEBUG() << "Match found.";
        QString email = checkQuery.value(0).toString();
        QString login = checkQuery.value(1).toString();
        QString password = checkQuery.value(2).toString();
        const common::BasicUser newUser(login, password, email);
        common::BasicUser insertedUser = insertNewUser(newUser);
        return insertedUser;
    }
    else
    {
        DEBUG() << "No matching users.";
        return common::BasicUser(NULL);;
    }
}


bool QueryExecutor::deleteTmpUser(const QString &token)
{
    PerformanceCounter counter("QueryExecutor::deleteTmpUser");
    bool result;
    QSqlQuery deleteSignupQuery=makeQuery();
    deleteSignupQuery.prepare("delete from signups where registration_token = :token;");
    deleteSignupQuery.bindValue(":token", token);
    DEBUG() << "Deleting: %s"<< deleteSignupQuery.lastQuery();

    transaction();

    result = deleteSignupQuery.exec();
    if(!result)
    {
        DEBUG() << "Rollback for deleteSignup sql query";
        rollback();
    }
    else
    {
        DEBUG() << "Commit for deleteSignup sql query";
        commit();
    }
    return result;
}


common::BasicUser QueryExecutor::insertNewUser(const common::BasicUser& user)
{
    PerformanceCounter counter("QueryExecutor::insertNewUser");
    bool result;
    QSqlQuery newUserQuery=makeQuery();
    qlonglong newId = nextUserKey();
    //  DEBUG() << "newToken = %s",newToken.toStdString().c_str());
    newUserQuery.prepare("insert into users (id,email,login,password) values(:id,:email,:login,:password);");
    newUserQuery.bindValue(":id",newId);
    newUserQuery.bindValue(":email",user.getEmail());
    newUserQuery.bindValue(":login",user.getLogin());
    newUserQuery.bindValue(":password",user.getPassword());
    transaction();

    result=newUserQuery.exec();
    common::BasicUser newUser = common::BasicUser(NULL);
    if(!result)
    {
        DEBUG() << "Rollback for NewUser sql query";
        rollback();
    }else
    {
        DEBUG() << "Commit for NewUser sql query";
        newUser = common::BasicUser(user.getLogin(),user.getPassword(),user.getEmail());
        commit();
    }
    return newUser;
}


bool QueryExecutor::subscribeChannel(const common::BasicUser& user,const Channel& channel)
{
    PerformanceCounter counter("QueryExecutor::subscribeChannel");
    bool result;
    QSqlQuery insertNewSubscribtion=makeQuery();
    insertNewSubscribtion.prepare("insert into subscribe (channel_id,user_id) values(:channel_id,:user_id);");
    insertNewSubscribtion.bindValue(":channel_id",channel.getId());
    insertNewSubscribtion.bindValue(":user_id",user.getId());
    DEBUG() << "Subscribing "<<user.getLogin()<<" (Id = "<<user.getId()
             <<") for "<<channel.getName()<<" (Id = "<<channel.getId() <<")";

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
    deleteSubscribtion.prepare("delete from subscribe where channel_id = :channel_id AND user_id = :user_id;");
    deleteSubscribtion.bindValue(":channel_id",channel.getId());
    deleteSubscribtion.bindValue(":user_id",user.getId());
    DEBUG() << "Unsubscribing " << user.getLogin() << " (Id = " << user.getId()
             << ") for " << channel.getName() << " (Id = " << channel.getId() <<")";

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
    DEBUG() << "Deleting: id = " << user.getId();
    deleteUserQuery.prepare("delete from users where id = :id;");
    deleteUserQuery.bindValue(":id",user.getId() );

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
    DEBUG() <<  "Updating password for user with id:" << user.getId();
    query.prepare("update users set password = :pwd where id = :id;");
    query.bindValue(":pwd", password);
    query.bindValue(":id", user.getId());

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


void QueryExecutor::checkTmpUsers()
{
    QSqlQuery checkQuery=makeQuery();
    QSqlQuery deleteQuery=makeQuery();
    DEBUG() << "checkTmpUsers query is running now...";
    // Sending emails to new users
    checkQuery.exec("select id, email, registration_token from signups where sent = false;");
    while (checkQuery.next())
    {
        qlonglong id = checkQuery.value(0).toLongLong();
        QString email = checkQuery.value(1).toString();
        QString token = checkQuery.value(2).toString();

        DEBUG() <<  "Process registration confirmation is started... ";
        EmailMessage message(email);
        message.sendAsRegistrationLetter(token);
        DEBUG() <<  "Process registration confirmation finished... ";

        QSqlQuery updateQuery=makeQuery();
        updateQuery.prepare("update signups set sent = true where id = :id;");
        updateQuery.bindValue(":id", id);
        bool result = updateQuery.exec();
        transaction();
        if(!result)
        {
            DEBUG() << "Rollback for CheckTmpUser sql query";
            rollback();
        }
        else
        {
            DEBUG() << "Commit for CheckTmpUser sql query";
            commit();
        }
    }

    // Deleting old signups
    QString strQuery;

    QString timelife = SettingsStorage::getValue("registration/tmp_user_timelife", QVariant(DEFAULT_TMP_USER_TIMELIFE)).toString();

    strQuery.append("select id from signups where (now() - datetime) >= INTERVAL '");
    strQuery.append(timelife);
    strQuery.append("';");
    checkQuery.exec(strQuery.toStdString().c_str());
    while (checkQuery.next())
    {
        qlonglong id = checkQuery.value(0).toLongLong();
        deleteQuery.prepare("delete from signups where id = :id;");
        deleteQuery.bindValue(":id", id);
        DEBUG() << "Deleting: " << deleteQuery.lastQuery();
        transaction();
        bool result = deleteQuery.exec();
        if(!result)
        {
            DEBUG() << "Rollback for DeleteTmpUser sql query";
            rollback();
        }
        else
        {
            DEBUG() << "Commit for DeleteTmpUser sql query";
            commit();
        }
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

QList<Channel> QueryExecutor::getChannelsByUser(const common::BasicUser &/*user*/)
{
    QList<Channel> r;

    NOT_IMPLEMENTED();

    return r;
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
        QDateTime time = query.record().value("time").toDateTime().toTimeSpec(Qt::LocalTime);
        qreal latitude = query.record().value("latitude").toReal();
        qreal altitude = query.record().value("altitude").toReal();
        qreal longitude = query.record().value("longitude").toReal();
        QString label = query.record().value("label").toString();
        QString description = query.record().value("description").toString();
        QString url = query.record().value("url").toString();

        //        qlonglong userId = query.record().value("user_id").toLongLong();
        //        qlonglong channelId = query.record().value("channel_id").toLongLong();

        Tag tag(altitude,latitude,longitude,label,description,url,time);
        container.push_back(tag);
    }
    return container;
}

QList<Tag> QueryExecutor::loadTags(const Channel &channel)
{
    QList<Tag> container;
    qlonglong channelId=getChannelIdByName(channel.getName());

    QSqlQuery query=makeQuery();
    query.exec(QString("select time, altitude, latitude, longitude, label, description, url, user_id, channel_id "
                       "from tag where channel_id = %1 order by time;").arg(channelId));
    while (query.next())
    {
        QDateTime time = query.record().value("time").toDateTime().toTimeSpec(Qt::LocalTime);
        qreal latitude = query.record().value("latitude").toReal();
        qreal altitude = query.record().value("altitude").toReal();
        qreal longitude = query.record().value("longitude").toReal();
        QString label = query.record().value("label").toString();
        QString description = query.record().value("description").toString();
        QString url = query.record().value("url").toString();

        //        qlonglong userId = query.record().value("user_id").toLongLong();
        //        qlonglong channelId = query.record().value("channel_id").toLongLong();

        Tag tag(altitude,latitude,longitude,label,description,url,time);
        container.push_back(tag);
    }
    return container;
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
        if(Core::MetaCache::checkUser(user))
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


void QueryExecutor::updateReflections(DataMarks &, QList<common::BasicUser> &, Channels &, Sessions &)
{
    Q_ASSERT(false);
#if 0 // GT-765
    {
        QSqlQuery query=makeQuery();
        query.exec("select user_id, channel_id from subscribe;");
        while (query.next())
        {
            qlonglong user_id = query.record().value("user_id").toLongLong();
            qlonglong channel_id = query.record().value("channel_id").toLongLong();
            users.item(user_id)->subscribe(channels.item(channel_id));
        }
    }
    {
        QSqlQuery query=makeQuery();
        query.exec("select tag_id, channel_id from tags;");
        while (query.next())
        {
            qlonglong tag_id = query.record().value("tag_id").toLongLong();
            qlonglong channel_id = query.record().value("channel_id").toLongLong();

            Channel channel = channels.item(channel_id);
            QSharedPointer<DataMark> tag = tags.item(tag_id);

            tag->setChannel(channel);
        }
    }

    for(int i=0; i<tags.size(); i++)
    {
        tags[i]->setUser(users.item(tags.at(i).dynamicCast<DbDataMark>()->getUserId()));
        tags[i]->setChannel(channels.item(tags.at(i).dynamicCast<DbDataMark>()->getChannelId()));
    }

    for(int i=0; i<sessions.size(); i++)
    {
        sessions[i]->setUser(users.item(sessions[i]->getUser()->getId()));
    }
#endif
}

qlonglong QueryExecutor::getUserIdByName(const QString &name)
{
    QSqlQuery query=makeQuery();
    DEBUG() << "getUserIdByName " << name;

    QString qry=QString("select id from users where login='%1';").arg(name);
    DEBUG() << "qry:" << qry;
    query.exec(qry);
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

    query.exec(QString("select id from channels where name=%1;").arg(name));
    qlonglong id =-1;

    if(!query.next())
    {
        qCritical() << "Cannot find existing channel " << name;
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
    QString q=QString("select name, description, url from channel where name='%1';").arg(name);

    query.exec(q);

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


qlonglong QueryExecutor::getFactTransactionNumber()
{
    QSqlQuery query=makeQuery();

    query.exec("select tup_inserted ,tup_updated ,tup_deleted from  pg_stat_database where datname='geo2tag';");
    query.next();
    qlonglong factCount = query.record().value("tup_inserted").toLongLong() +
            query.record().value("tup_updated").toLongLong() +
            query.record().value("tup_deleted").toLongLong();

    return factCount;
}

QSqlQuery QueryExecutor::makeQuery()
{
    return QSqlQuery(QSqlDatabase::database());
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

QueryExecutor *QueryExecutor::instance()
{
    static QueryExecutor instance;
    return &instance;
}
