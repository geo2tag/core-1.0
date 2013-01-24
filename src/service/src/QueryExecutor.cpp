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


const QString QueryExecutor::generateNewToken(const QString& email, const QString& login,const QString& password) const
{
    QString log=login+password+email;
    QByteArray toHash(log.toUtf8());
    toHash=QCryptographicHash::hash(log.toUtf8(),QCryptographicHash::Md5);
    QString result(toHash.toHex());
    qDebug() << "Token = %s" << result;
    return result;
}


const QString QueryExecutor::generateNewToken(const QString& accessTime, const QString& email, const QString& login,const QString& password) const
{
    QString log=login+password+email+accessTime;
    QByteArray toHash(log.toUtf8());
    toHash=QCryptographicHash::hash(log.toUtf8(),QCryptographicHash::Md5);
    QString result(toHash.toHex());
    qDebug() << "TOken = %s" << result;
    return result;
}


QSharedPointer<Tag> QueryExecutor::insertNewTag(const QSharedPointer<Tag>& tag)
{
    PerformanceCounter counter("QueryExecutor::insertNewTag");
    bool result;
    qlonglong newId = nextTagKey();

    qDebug() <<  QString("insertNewTag-start-").append(QString::number(newId));

    QSqlQuery newTagQuery=makeQuery();
    newTagQuery.prepare("insert into tag (altitude , latitude, longitude, label, description, url, user_id, time, id, channel_id) "
                        "         values(:altitude,:latitude,:longitude,:label,:description,:url,:user_id,:time,:id, :channel_id);");
    newTagQuery.bindValue(":altitude", tag->getAltitude());
    newTagQuery.bindValue(":latitude", tag->getLatitude());
    newTagQuery.bindValue(":longitude", tag->getLongitude());
    newTagQuery.bindValue(":label", tag->getLabel().isEmpty()? "New Mark" : tag->getLabel());
    newTagQuery.bindValue(":description", tag->getDescription());
    newTagQuery.bindValue(":url", tag->getUrl());
    newTagQuery.bindValue(":user_id", tag->getUser()->getId());
    newTagQuery.bindValue(":channel_id", tag->getChannel()->getId());
    newTagQuery.bindValue(":time", tag->getTime().toUTC());
    newTagQuery.bindValue(":id", newId);

    transaction();

    result = newTagQuery.exec();
    if(!result)
    {
        rollback();
        qDebug() <<  "Rollback for NewTag sql query";
        return QSharedPointer<Tag>(NULL);
    }

    commit();

    QSharedPointer<Tag> t(
                new DbDataMark(newId, tag->getAltitude(), tag->getLatitude(),
                               tag->getLongitude(), tag->getLabel(),
                               tag->getDescription(), tag->getUrl(),
                               tag->getTime(), tag->getUser()->getId(),tag->getChannel()->getId()));
    t->setUser(tag->getUser());
    t->setChannel(tag->getChannel());

    return t;
}


QSharedPointer<Channel> QueryExecutor::insertNewChannel(const Channel &channel)
{
    PerformanceCounter counter("QueryExecutor::insertNewChannel");
    bool result;
    QSqlQuery newChannelQuery=makeQuery();
    qlonglong newId = nextChannelKey();
    qDebug() << "NewId ready, now preparing sql query for adding new channel";
    newChannelQuery.prepare("insert into channel (id,name,description,url,owner_id) values(:id,:name,:description,:url,:owner_id);");
    newChannelQuery.bindValue(":id",newId);
    newChannelQuery.bindValue(":name",channel.getName());
    newChannelQuery.bindValue(":description",channel.getDescription());
    newChannelQuery.bindValue(":url",channel.getUrl());
    newChannelQuery.bindValue(":owner_id",channel.getOwner()->getId());

    transaction();

    result=newChannelQuery.exec();
    if(!result)
    {
        qDebug() << "Rollback for NewChannel sql query";
        rollback();
        return QSharedPointer<Channel>(NULL);
    }else
        qDebug() << "Commit for NewChannel sql query - insert in table channel";

    commit();

    QSharedPointer<DbChannel> newChannel(new DbChannel(newId,channel.getName(),channel.getDescription(),channel.getUrl(),channel.getOwner()));
    return newChannel;
}


bool QueryExecutor::doesTmpUserExist(const QSharedPointer<common::User> &user)
{
    PerformanceCounter counter("QueryExecutor::doesTmpUserExist");
    QSqlQuery query=makeQuery();
    query.prepare("select * from signups where login = :login or email = :email;");
    query.bindValue(":login",user->getLogin());
    query.bindValue(":email",user->getEmail());
    qDebug() << "Selecting: %s" <<  query.lastQuery();

    query.exec();

    if (query.next())
    {
        return true;
    }
    else
    {
        qDebug() << "No matching users.";
        return false;
    }
}


bool QueryExecutor::doesUserWithGivenEmailExist(const QSharedPointer<common::User> &user)
{
    PerformanceCounter counter("QueryExecutor::doesUserWithGivenEmailExist");
    QSqlQuery query=makeQuery();
    qDebug() <<  "Checking of user existence in users by email: "<<user->getEmail();

    query.prepare("select id from users where email = :email;");
    query.bindValue(":email", user->getEmail());
    qDebug() << "Selecting: " <<  query.lastQuery();
    query.exec();

    if (query.next())
    {
        qDebug() << "Match found.";
        return true;
    }
    else
    {
        qDebug() << "No matching users.";
        return false;
    }
}


bool QueryExecutor::deleteTmpUser(const QSharedPointer<common::User> &user)
{
    PerformanceCounter counter("QueryExecutor::deleteTmpUser");
    bool result;
    QSqlQuery deleteSignupQuery=makeQuery();
    deleteSignupQuery.prepare("delete from signups where login = :login;");
    deleteSignupQuery.bindValue(":login",user->getLogin() );
    qDebug() << "Deleting: "<< deleteSignupQuery.lastQuery();

    transaction();

    result = deleteSignupQuery.exec();
    if(!result)
    {
        qDebug() << "Rollback for deleteSignup sql query";
        rollback();
    }
    else
    {
        qDebug() << "Commit for deleteSignup sql query";
        commit();
    }
    return result;
}


const QString QueryExecutor::insertNewTmpUser(const QSharedPointer<common::User> &user)
{
    PerformanceCounter counter("QueryExecutor::insertNewTmpUser");
    bool result;
    QSqlQuery newSignupQuery=makeQuery();
    qlonglong newId = nextUserKey();
    qDebug() << "Generating token for new signup " << user->getLogin() << user->getPassword();
    QString newToken = generateNewToken(user->getEmail(), user->getLogin(),user->getPassword());
    newSignupQuery.prepare("insert into signups (id,email,login,password,registration_token,sent) values(:id,:email,:login,:password,:r_token,:sent);");
    newSignupQuery.bindValue(":id", newId);
    newSignupQuery.bindValue(":email", user->getEmail());
    newSignupQuery.bindValue(":login", user->getLogin());
    newSignupQuery.bindValue(":password", user->getPassword());
    newSignupQuery.bindValue(":r_token", newToken);
    newSignupQuery.bindValue(":sent", FALSE);

    transaction();

    result = newSignupQuery.exec();
    if(!result)
    {
        qDebug() << "Rollback for NewSignup sql query";
        rollback();
        return QString("");
    }
    qDebug() << "Commit for NewSignup sql query";
    commit();

    return newToken;
}


bool QueryExecutor::doesRegistrationTokenExist(const QString &token)
{
    PerformanceCounter counter("QueryExecutor::doesRegistrationTokenExist");
    QSqlQuery query=makeQuery();
    qDebug() <<  "Checking of user existence in signups by token:" <<  token.toStdString().c_str();

    query.prepare("select id from signups where registration_token = :token;");
    query.bindValue(":token", token);
    qDebug() << "Selecting: "<< query.lastQuery().toStdString().c_str();
    query.exec();

    if (query.next())
    {
        qDebug() << "Match found.";
        return true;
    }
    else
    {
        qDebug() << "No matching users.";
        return false;
    }
}


QSharedPointer<common::User> QueryExecutor::insertTmpUserIntoUsers(const QString &token)
{
    PerformanceCounter counter("QueryExecutor::insertTmpUserIntoUsers");
    QSqlQuery checkQuery=makeQuery();
    qDebug() <<  "Checking of user existence in signups by token: %s"<< token;

    checkQuery.prepare("select email, login, password from signups where registration_token = :token;");
    checkQuery.bindValue(":token", token);
    qDebug() << "Selecting: %s"<< checkQuery.lastQuery();
    checkQuery.exec();

    if (checkQuery.next())
    {
        qDebug() << "Match found.";
        QString email = checkQuery.value(0).toString();
        QString login = checkQuery.value(1).toString();
        QString password = checkQuery.value(2).toString();
        const QSharedPointer<common::User> newUser(new common::User(login, password, email));
        QSharedPointer<common::User> insertedUser = insertNewUser(newUser);
        return insertedUser;
    }
    else
    {
        qDebug() << "No matching users.";
        return QSharedPointer<common::User>(NULL);;
    }
}


bool QueryExecutor::deleteTmpUser(const QString &token)
{
    PerformanceCounter counter("QueryExecutor::deleteTmpUser");
    bool result;
    QSqlQuery deleteSignupQuery=makeQuery();
    deleteSignupQuery.prepare("delete from signups where registration_token = :token;");
    deleteSignupQuery.bindValue(":token", token);
    qDebug() << "Deleting: %s"<< deleteSignupQuery.lastQuery();

    transaction();

    result = deleteSignupQuery.exec();
    if(!result)
    {
        qDebug() << "Rollback for deleteSignup sql query";
        rollback();
    }
    else
    {
        qDebug() << "Commit for deleteSignup sql query";
        commit();
    }
    return result;
}


QSharedPointer<common::User> QueryExecutor::insertNewUser(const QSharedPointer<common::User>& user)
{
    PerformanceCounter counter("QueryExecutor::insertNewUser");
    bool result;
    QSqlQuery newUserQuery=makeQuery();
    qlonglong newId = nextUserKey();
    //  qDebug() << "newToken = %s",newToken.toStdString().c_str());
    newUserQuery.prepare("insert into users (id,email,login,password) values(:id,:email,:login,:password);");
    newUserQuery.bindValue(":id",newId);
    newUserQuery.bindValue(":email",user->getEmail());
    newUserQuery.bindValue(":login",user->getLogin());
    newUserQuery.bindValue(":password",user->getPassword());
    transaction();

    result=newUserQuery.exec();
    QSharedPointer<common::User> newUser = QSharedPointer<common::User>(NULL);
    if(!result)
    {
        qDebug() << "Rollback for NewUser sql query";
        rollback();
    }else
    {
        qDebug() << "Commit for NewUser sql query";
        newUser = QSharedPointer<common::User>(new DbUser(user->getLogin(),user->getPassword(),user->getEmail(),newId));
        commit();
    }
    return newUser;
}


bool QueryExecutor::subscribeChannel(const QSharedPointer<common::User>& user,const QSharedPointer<Channel>& channel)
{
    PerformanceCounter counter("QueryExecutor::subscribeChannel");
    bool result;
    QSqlQuery insertNewSubscribtion=makeQuery();
    insertNewSubscribtion.prepare("insert into subscribe (channel_id,user_id) values(:channel_id,:user_id);");
    insertNewSubscribtion.bindValue(":channel_id",channel->getId());
    insertNewSubscribtion.bindValue(":user_id",user->getId());
    qDebug() << "Subscribing "<<user->getLogin()<<" (Id = "<<user->getId()
             <<") for "<<channel->getName()<<" (Id = "<<channel->getId() <<")";

    transaction();
    result=insertNewSubscribtion.exec();
    if(!result)
    {
        qDebug() << "Rollback for subscribeChannel sql query";
        rollback();
    }else
    {
        qDebug() << "Commit for subscribeChannel sql query";
        commit();
    }
    return result;
}


bool QueryExecutor::unsubscribeChannel(const QSharedPointer<common::User>& user,const QSharedPointer<Channel>& channel)
{
    PerformanceCounter counter("QueryExecutor::unsubscribeChannel");
    bool result;
    QSqlQuery deleteSubscribtion=makeQuery();
    deleteSubscribtion.prepare("delete from subscribe where channel_id = :channel_id AND user_id = :user_id;");
    deleteSubscribtion.bindValue(":channel_id",channel->getId());
    deleteSubscribtion.bindValue(":user_id",user->getId());
    qDebug() << "Unsubscribing " << user->getLogin() << " (Id = " << user->getId()
             << ") for " << channel->getName() << " (Id = " << channel->getId() <<")";

    transaction();

    result=deleteSubscribtion.exec();
    if(!result)
    {
        qDebug() << "Rollback for unsubscribeChannel sql query";
        rollback();
    }else
    {
        qDebug() << "Commit for unsubscribeChannel sql query";
        commit();
    }
    return result;
}


bool QueryExecutor::deleteUser(const QSharedPointer<common::User> &user)
{
    PerformanceCounter counter("QueryExecutor::deleteUser");
    bool result;
    QSqlQuery deleteUserQuery=makeQuery();
    qDebug() << "Deleting: id = " << user->getId();
    deleteUserQuery.prepare("delete from users where id = :id;");
    deleteUserQuery.bindValue(":id",user->getId() );

    transaction();

    result = deleteUserQuery.exec();
    if(!result)
    {
        qDebug() << "Rollback for deleteUser sql query";
        rollback();
    }
    else
    {
        qDebug() << "Commit for deleteUser sql query";
        commit();
    }
    return result;
}


QSharedPointer<common::User> QueryExecutor::updateUserPassword(const QSharedPointer<common::User>& user, const QString& password)
{
    QSqlQuery query=makeQuery();
    qDebug() <<  "Updating password for user with id:" << user->getId();
    query.prepare("update users set password = :pwd where id = :id;");
    query.bindValue(":pwd", password);
    query.bindValue(":id", user->getId());

    transaction();

    bool result = query.exec();
    if (!result)
    {
        qDebug() << "Rollback for updateUserPassword sql query";
        rollback();
        return QSharedPointer<common::User>(NULL);
    }
    else
    {
        qDebug() << "Commit for updateUsersPassword sql query";
        commit();
        user->setPassword(password);
        return user;
    }
}


Session QueryExecutor::insertNewSession(const common::BasicUser &user)
{
    QSqlQuery query=makeQuery();
    qlonglong newId = nextSessionKey();
    QDateTime time=QDateTime::currentDateTimeUtc();
    QString newSessionToken = generateNewToken(QDateTime::currentDateTimeUtc().toString(),
                                               user.getEmail(),
                                               user.getLogin(),
                                               user.getPassword());

    qDebug() <<  "NewId ready, now preparing sql query for adding new session";
    query.prepare("insert into sessions (id, user_id, session_token, last_access_time) values (:id, :user_id, :token, :time);");
    query.bindValue(":id", newId);
    query.bindValue(":user_id", QueryExecutor::getUserIdByName(user.getLogin()));
    query.bindValue(":token", newSessionToken);
    query.bindValue(":time", time);

    transaction();

    bool result = query.exec();
    if (!result)
    {
        qDebug() << "Rollback for NewSession sql query";
        rollback();
        return Session();
    }
    else
    {
        qDebug() << "Commit for NewSession sql query - insert in table sessions";
        commit();
    }
    return Session(newSessionToken, time, user);
}


bool QueryExecutor::updateSession(Session &session)
{
    QSqlQuery query=makeQuery();
    QDateTime currentTime = QDateTime::currentDateTime();
    qDebug() <<  "Updating session with token: " << session.getSessionToken();

    query.prepare("update sessions set last_access_time = :time where session_token = :token;");
    query.bindValue(":time", currentTime);
    query.bindValue(":token", session.getSessionToken());

    transaction();

    bool result = query.exec();
    if (!result)
    {
        qDebug() << "Rollback for updateSession sql query";
        rollback();
        return false;
    }
    else
    {
        qDebug() << "Commit for updateSession sql query";
        commit();
        session.setLastAccessTime(currentTime);
        return true;
    }
}


bool QueryExecutor::deleteSession(const Session &session)
{
    QSqlQuery query=makeQuery();
    qDebug() <<  "Deleting session with token: " <<  session.getSessionToken();

    query.prepare("delete from sessions where session_token = :id;");
    query.bindValue(":id", session.getSessionToken());

    transaction();

    bool result = query.exec();
    if (!result)
    {
        qDebug() <<  "Rollback for deleteSession sql query";
        rollback();
        return false;
    }
    else
    {
        qDebug() <<  "Commit for deleteSession sql query";
        commit();
        return true;
    }
}


void QueryExecutor::checkTmpUsers()
{
    QSqlQuery checkQuery=makeQuery();
    QSqlQuery deleteQuery=makeQuery();
    qDebug() << "checkTmpUsers query is running now...";
    // Sending emails to new users
    checkQuery.exec("select id, email, registration_token from signups where sent = false;");
    while (checkQuery.next())
    {
        qlonglong id = checkQuery.value(0).toLongLong();
        QString email = checkQuery.value(1).toString();
        QString token = checkQuery.value(2).toString();

        qDebug() <<  "Process registration confirmation is started... ";
        EmailMessage message(email);
        message.sendAsRegistrationLetter(token);
        qDebug() <<  "Process registration confirmation finished... ";

        QSqlQuery updateQuery=makeQuery();
        updateQuery.prepare("update signups set sent = true where id = :id;");
        updateQuery.bindValue(":id", id);
        bool result = updateQuery.exec();
        transaction();
        if(!result)
        {
            qDebug() << "Rollback for CheckTmpUser sql query";
            rollback();
        }
        else
        {
            qDebug() << "Commit for CheckTmpUser sql query";
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
        qDebug() << "Deleting: " << deleteQuery.lastQuery();
        transaction();
        bool result = deleteQuery.exec();
        if(!result)
        {
            qDebug() << "Rollback for DeleteTmpUser sql query";
            rollback();
        }
        else
        {
            qDebug() << "Commit for DeleteTmpUser sql query";
            commit();
        }
    }
}


void QueryExecutor::checkSessions(UpdateThread* /*thread*/)
{
#if 0 // GT-765
    qDebug() << "checkSessions query is running now...";
    int timelife = SettingsStorage::getValue("general/session_timelife", QVariant(DEFAULT_SESSION_TIMELIFE)).toInt();
    for (int i = 0; i < thread->getSessionsContainer()->size(); i++)
    {
        QDateTime currentTime = QDateTime::currentDateTime().toUTC();
        //qDebug() <<  "Current time: %s", currentTime.toString().toStdString().c_str());
        QDateTime lastAccessTime = thread->getSessionsContainer()->at(i)->getLastAccessTime();
        //qDebug() <<  "Last access time: %s", lastAccessTime.toString().toStdString().c_str());
        if (lastAccessTime.addDays(timelife) <= currentTime)
        {
            QSqlQuery query=makeQuery();
            query.prepare("delete from sessions where id = :id;");
            query.bindValue(":id", thread->getSessionsContainer()->at(i)->getId());
            qDebug() << "Deleting: "<< query.lastQuery();
            transaction();
            bool result = query.exec();
            if (!result)
            {
                qDebug() <<  "Rollback for DeleteSession sql query";
                rollback();
            }
            else
            {
                qDebug() <<  "Commit for DeleteSession sql query";
                commit();
            }
            QWriteLocker(thread->getLock());
            thread->getSessionsContainer()->erase(thread->getSessionsContainer()->at((i)));
        }
    }
#endif
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


void QueryExecutor::loadChannels(Channels &container)
{
    QSqlQuery query=makeQuery();
    query.exec("select id, description, name, url, owner_id from channel order by id;");
    while (query.next())
    {
        qlonglong id = query.record().value("id").toLongLong();
        if(container.exist(id))
        {
            // skip record
            continue;
        }
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

        QSharedPointer<common::BasicUser> owner(new DbUser(login, passw, email, ownerId));
        QSharedPointer<DbChannel> pointer(new DbChannel(id,name,description,url,owner));
        container.push_back(pointer);
    }
}


void QueryExecutor::loadTags(DataMarks &container)
{
    QSqlQuery query=makeQuery();
    query.exec("select id, time, altitude, latitude, longitude, label, description, url, user_id, channel_id from tag order by time;");
    while (query.next())
    {
        qlonglong id = query.record().value("id").toLongLong();
        if(container.exist(id))
        {
            // skip record
            continue;
        }
        QDateTime time = query.record().value("time").toDateTime().toTimeSpec(Qt::LocalTime);
        //       // qDebug() <<  "loaded tag with time: %s milliseconds", time;
        qreal latitude = query.record().value("latitude").toReal();
        qreal altitude = query.record().value("altitude").toReal();
        qreal longitude = query.record().value("longitude").toReal();
        QString label = query.record().value("label").toString();
        QString description = query.record().value("description").toString();
        QString url = query.record().value("url").toString();
        qlonglong userId = query.record().value("user_id").toLongLong();
        qlonglong channelId = query.record().value("channel_id").toLongLong();

        DbDataMark *newMark = new DbDataMark(id,
                                             altitude,
                                             latitude,
                                             longitude,
                                             label,
                                             description,
                                             url,
                                             time,
                                             userId,
                                             channelId);
        QSharedPointer<DbDataMark> pointer(newMark);
        container.push_back(pointer);
    }
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
            qDebug() << "added session " << session.getSessionToken() << " for " << user;
        } else
        {
            qDebug() << "User " << user << " is not valid";
        }
    }
    return result;
}


void QueryExecutor::updateReflections(DataMarks &, common::Users &, Channels &, Sessions &)
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

            QSharedPointer<Channel> channel = channels.item(channel_id);
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

    query.exec(QString("select id from users where login=%1;").arg(name));
    qlonglong id =-1;

    if(!query.next())
    {
        qCritical() << "Callot find existing user " << name;
    }
    else
    {
        id = query.record().value("id").toLongLong();
    }

    return id;
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
