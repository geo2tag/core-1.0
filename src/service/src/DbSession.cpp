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
/*! ---------------------------------------------------------------
 *
 *
 * \file DbSession.cpp
 * \brief DbSession implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include "servicelogger.h"
#include <stdlib.h>
#include "defines.h"
#include "SettingsStorage.h"
#include "DbSession.h"
#include "EmailMessage.h"

#include "RegisterUserRequestJSON.h"
#include "RegisterUserResponseJSON.h"

#include "QuitSessionRequestJSON.h"
#include "QuitSessionResponseJSON.h"

#include "AddUserRequestJSON.h"
#include "AddUserResponseJSON.h"

#include "OwnedChannelsRequest.h"
#include "OwnedChannelsResponse.h"

#include "SubscribedChannelsRequestJSON.h"
#include "SubscribedChannelsResponseJSON.h"

#include "SubscribeChannelJSON.h"
#include "SubscribeChannelResponseJSON.h"


#include "UnsubscribeChannelRequestJSON.h"
#include "UnsubscribeChannelResponseJSON.h"

#include "RestorePasswordRequestJSON.h"
#include "RestorePasswordResponseJSON.h"

#include "DeleteUserRequestJSON.h"
#include "DeleteUserResponseJSON.h"

#include "ErrnoInfoResponseJSON.h"

#include "BuildResponseJSON.h"

#include "ChannelInternal.h"
#include "ErrnoTypes.h"

#include <QtSql>
#include <QMap>
#include <QRegExp>

#include "PerformanceCounter.h"
#include "Geo2tagDatabase.h"
#include "MetaCache.h"

namespace common
{
const QString DbObjectsCollection::error = QString("Error");
const QString DbObjectsCollection::ok = QString("Ok");

DbObjectsCollection::DbObjectsCollection()
{
    //Core processors
    m_processors.insert("login", &DbObjectsCollection::processLoginQuery);
    m_processors.insert("writeTag", &DbObjectsCollection::processWriteTagQuery);
    m_processors.insert("loadTags", &DbObjectsCollection::processLoadTagsQuery);
    m_processors.insert("addChannel", &DbObjectsCollection::processAddChannelQuery);
    m_processors.insert("channels", &DbObjectsCollection::processAvailableChannelsQuery);
    m_processors.insert("version", &DbObjectsCollection::processVersionQuery);
    m_processors.insert("quitSession", &DbObjectsCollection::processQuitSessionQuery);
    m_processors.insert("build", &DbObjectsCollection::processBuildQuery);
    m_processors.insert("errnoInfo", &DbObjectsCollection::processGetErrnoInfo);


    m_processors.insert("subscribe", &DbObjectsCollection::processSubscribeQuery);
    m_processors.insert("unsubscribe", &DbObjectsCollection::processUnsubscribeQuery);
    m_processors.insert("subscribed", &DbObjectsCollection::processSubscribedChannelsQuery);


    m_processors.insert("addUser", &DbObjectsCollection::processAddUserQuery);
    m_processors.insert("deleteUser", &DbObjectsCollection::processDeleteUserQuery);
    m_processors.insert("owned", &DbObjectsCollection::processOwnedChannelsQuery);



    m_processors.insert("filterCircle", &DbObjectsCollection::processFilterCircleQuery);
    m_processors.insert("filterCylinder", &DbObjectsCollection::processFilterCylinderQuery);
    m_processors.insert("filterPolygon", &DbObjectsCollection::processFilterPolygonQuery);
    m_processors.insert("filterRectangle", &DbObjectsCollection::processFilterRectangleQuery);
    m_processors.insert("filterBox", &DbObjectsCollection::processFilterBoxQuery);
    m_processors.insert("filterFence", &DbObjectsCollection::processFilterFenceQuery);
    m_processors.insert("filterChannel", &DbObjectsCollection::processFilterChannelQuery);

//    m_processors.insert("registerUser", &DbObjectsCollection::processRegisterUserQuery);
//    m_processors.insert("restorePassword", &DbObjectsCollection::processRestorePasswordQuery);
//    m_processors.insert("confirmRegistration-*", &DbObjectsCollection::processFilterFenceQuery);


    WARNING() << "Platform version: " << getPlatformVersion();
    WARNING() << "Platform build info: " << getPlatformBuildInfo();


    //GT-817 Now is only QPSQL base is supported
    QSqlDatabase database = QSqlDatabase::addDatabase("QPSQL");

    QVariant defaultName("COULD_NOT_READ_CONFIG");
    QString host=SettingsStorage::getValue("database/host",defaultName).toString();
    QString name=SettingsStorage::getValue("database/name",defaultName).toString();
    QString user=SettingsStorage::getValue("database/user",defaultName).toString();
    QString pass=SettingsStorage::getValue("database/password",defaultName).toString();

    database.setHostName(host);
    database.setDatabaseName(name);
    database.setUserName(user);
    database.setPassword(pass);

    DEBUG() << "Connecting to " << database.databaseName() << ", options= " << database.connectOptions();
    DEBUG() << "database.open()=" << database.open();
}

DbObjectsCollection& DbObjectsCollection::getInstance()
{
    static DbObjectsCollection s;
    return s;
}

DbObjectsCollection::~DbObjectsCollection()
{
}

QString DbObjectsCollection::getPlatformVersion()
{
    QString version = SettingsStorage::getValue("common/version",QString("UNKNOWN VERSION")).toString();
    return version;
}

QString DbObjectsCollection::getPlatformBuildInfo()
{
    QString build = SettingsStorage::getValue("common/build",QString("UNKNOWN BUILD")).toString();
    return build;
}

QByteArray DbObjectsCollection::process(const QString& queryType, const QByteArray& body)
{

    QList<QString> queries = m_processors.uniqueKeys();
    for (int i=0;i<queries.size();i++)
    {
        if (queryType.compare(queries[i],Qt::CaseInsensitive) == 0)
        {

            ProcessMethod method = m_processors.value(queries[i]);
            DEBUG() << "calling " << queryType << " processor " << body;
            QByteArray aa;
            PerformanceCounter a(queryType.toStdString());
            aa = (*this.*method)(body);
            return aa;
        }
    }

    // Extra code for extracting token from url !
    QRegExp rx("confirmRegistration-([a-zA-Z0-9]+)");

    if (rx.exactMatch(queryType))
    {
        DEBUG() <<  "Pattern matched!";
        const QString token = rx.cap(1);
        ProcessMethodWithStr method = &DbObjectsCollection::processConfirmRegistrationQuery;
        QByteArray aa;
        PerformanceCounter a("confirmRegistration");
        aa = (*this.*method)(token);
        return aa;
    }
    else
    {
        WARNING() << "Command \"" << queryType << "\" not found. Maybe not supported in this build";
    }
    // end of extra code !

    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");
    DefaultResponseJSON response;
    response.setErrno(INCORRECT_QUERY_NAME_ERROR);

    answer.append(response.getJson());
    DEBUG() << "answer: " <<  answer.data();
    return answer;

}

// Check password quality params - length, usage of different symbol groups ([-=+_*&^%$#@a-z],[A-Z],[0-9])
bool DbObjectsCollection::checkPasswordQuality(const QString& password) const
{
    // If check is not enabled in config - all passwords are good
    bool checkEnabled = SettingsStorage::getValue("security/password_quality_check", QVariant(DEFAULT_PASSWORD_QUALITY_CHECK)).toBool();
    if (!checkEnabled) return true;

    if ( password.size() < MINIMAL_PASSWORD_LENGTH )
        return false;

    // Check that all symbol groups below are used in password
    QRegExp smallLetters("[-=+_*&^%$#@a-z]+");
    QRegExp bigLetters("[A-Z]+");
    QRegExp numbers("[0-9]+");

    if (smallLetters.indexIn(password)==-1 || bigLetters.indexIn(password)==-1 || numbers.indexIn(password)==-1 )
        return false;

    return true;
}

const QString DbObjectsCollection::getPasswordHash(const common::BasicUser& user) const
{
    return getPasswordHash(user.getLogin(),user.getPassword());
}

const QString DbObjectsCollection::getPasswordHash(const QString & login,const QString & password) const
{
    QString startStr = login + password + PASSWORD_SALT;
    QByteArray toHash = QCryptographicHash::hash(startStr.toUtf8(),QCryptographicHash::Sha1);
    QString result(toHash.toHex());
    DEBUG() << "Calculating hash for password " << result;
    return result;
}

const QString DbObjectsCollection::generateNewPassword(const common::BasicUser& user) const
{
    QDateTime time = QDateTime::currentDateTime().toUTC();
    QString log = time.toString() + user.getEmail() + user.getPassword();
    QByteArray toHash(log.toUtf8());
    toHash=QCryptographicHash::hash(log.toUtf8(),QCryptographicHash::Md5);
    QString result(toHash.toHex());
    DEBUG() << "Password = " << result;
    return result;
}

//BasicUser DbObjectsCollection::findUser(const common::BasicUser &dummyUser) const
//{
//    common::BasicUser realUser;      // Null pointer

//    QList<User> currentUsers = Core::MetaCache::getUsers();
//    DEBUG() << "checking user key: " << dummyUser->getLogin() << " from " << currentUsers.size() <<" known users";


//    if (!dummyUser->getLogin().isEmpty() && !dummyUser->getPassword().isEmpty())
//    {
//        for(int i=0; i<currentUsers.size(); i++)
//        {
//            if(QString::compare(currentUsers.at(i)->getLogin(), dummyUser->getLogin(), Qt::CaseInsensitive) == 0
//                    &&
//                    currentUsers.at(i)->getPassword() == getPasswordHash(currentUsers.at(i)->getLogin(),dummyUser->getPassword()))
//                return currentUsers.at(i);
//        }
//    }
//    return realUser;
//}


//Session DbObjectsCollection::findSession(const Session& dummySession) const
//{
//    QVector< Session > currentSessions = m_sessionsContainer->vector();
//    DEBUG() << "checking session key: " << dummySession->getSessionToken()<<" from " << currentSessions.size() << " known sessions";
//    if (!dummySession->getSessionToken().isEmpty())
//    {
//        for (int i=0; i<currentSessions.size(); i++)
//        {
//            if(currentSessions.at(i)->getSessionToken() == dummySession->getSessionToken())
//                return currentSessions.at(i);
//        }
//    }
//    return Session(NULL);
//}

//Session DbObjectsCollection::findSessionForUser(const BasicUser &user) const
//{
//    QVector< Session > currentSessions = m_sessionsContainer->vector();
//    DEBUG() << "checking of session existence for user with name:" <<  user.getLogin();

//    Session s;
//    foreach(s,currentSessions)
//    {
//        if (QString::compare(s->getUser()->getLogin(), user.getLogin(), Qt::CaseInsensitive) == 0
//                &&
//                s->getUser()->getPassword() == user.getPassword())
//            return s;
//    }
//    return Session(NULL);
//}

//QByteArray DbObjectsCollection::processRegisterUserQuery(const QByteArray &data)
//{
//    Q_ASSERT(false); // Not_implemented

//    RegisterUserRequestJSON request;
//    RegisterUserResponseJSON response;
//    QByteArray answer;
//    answer.append("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");
//    if (!request.parseJson(data))
//    {
//        response.setErrno(INCORRECT_JSON_ERROR);
//        answer.append(response.getJson());
//        return answer;
//    }
//    //    common::BasicUser newTmpUser = request.getUsers()->at(0);
//    //    QVector<common::BasicUser > currentUsers = m_usersContainer->vector();
//    //    for(int i=0; i<currentUsers.size(); i++)
//    //    {
//    //        if(QString::compare(currentUsers.at(i)->getLogin(), newTmpUser->getLogin(), Qt::CaseInsensitive) == 0)
//    //        {
//    //            response.setErrno(USER_ALREADY_EXIST_ERROR);
//    //            answer.append(response.getJson());
//    //            DEBUG() << "answer: " <<  answer.data();
//    //            return answer;
//    //        }
//    //    }

//    //    if(QueryExecutor::instance()->doesUserWithGivenEmailExist(newTmpUser))
//    //    {
//    //        response.setErrno(EMAIL_ALREADY_EXIST_ERROR);
//    //        answer.append(response.getJson());
//    //        return answer;
//    //    }

//    //    if(QueryExecutor::instance()->doesTmpUserExist(newTmpUser))
//    //    {
//    //        response.setErrno(TMP_USER_ALREADY_EXIST_ERROR);
//    //        answer.append(response.getJson());
//    //        return answer;
//    //    }

//    //    if ( !checkPasswordQuality(newTmpUser->getPassword()) )
//    //    {
//    //        response.setErrno(WEAK_PASSWORD_ERROR);
//    //        answer.append(response.getJson());
//    //        return answer;
//    //    }

//    //    // Only password hashes are stored, so we change password of this user by password hash
//    //    newTmpUser->setPassword(getPasswordHash(newTmpUser));
//    //    QString confirmToken = QueryExecutor::instance()->insertNewTmpUser(newTmpUser);
//    //    if(confirmToken.isEmpty())
//    //    {
//    //        response.setErrno(INTERNAL_DB_ERROR);
//    //        answer.append(response.getJson());
//    //        DEBUG() << "answer: " <<  answer.data();
//    //        return answer;
//    //    }

//    //    response.setErrno(SUCCESS);

//    //    QString serverUrl = SettingsStorage::getValue("common/server_url", QVariant(DEFAULT_SERVER)).toString();
//    //    response.setConfirmUrl(serverUrl+QString("service/confirmRegistration-")+confirmToken);
//    //    answer.append(response.getJson());
//    //    DEBUG() << "answer: " <<  answer.data();
//    return answer;
//}

QByteArray DbObjectsCollection::processConfirmRegistrationQuery(const QString &/*registrationToken*/)
{
    QByteArray answer;
    answer.append("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");
    Q_ASSERT(false); //NOT implemented
    //    bool tokenExists = QueryExecutor::instance()->doesRegistrationTokenExist(registrationToken);
    //    if (!tokenExists)
    //    {
    //        answer.append("Given token doesn't exist!");
    //        return answer;
    //    }

    //    common::BasicUser newUser = QueryExecutor::instance()->insertTmpUserIntoUsers(registrationToken);

    //    if (!newUser.isNull())
    //    {
    //        QueryExecutor::instance()->deleteTmpUser(registrationToken);
    //        m_usersContainer->push_back(newUser);
    //        answer.append("Congratulations!");
    //    }
    //    else
    //    {
    //        answer.append("Attempt of inserting user has failed!");
    //    }
    return answer;
}


QByteArray DbObjectsCollection::processQuitSessionQuery(const QByteArray &/*data*/)
{
    NOT_IMPLEMENTED();
    QuitSessionRequestJSON request;
    QuitSessionResponseJSON response;
    QByteArray answer;
    answer.append("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");
#if 0

    if (!request.parseJson(data))
    {
        response.setErrno(INCORRECT_JSON_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    QString sessionToken = request.getSessionToken();
    DEBUG() << "Searching of session with token: " << sessionToken;
    Session session = findSession();

    if(session.isNull())
    {
        DEBUG() <<  "Session hasn't been found.";
        response.setErrno(WRONG_TOKEN_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    DEBUG() <<  "Session has been found. Deleting...";
    DEBUG() <<  "Number of sessions before deleting: "<< m_sessionsContainer->size();
    bool result = QueryExecutor::instance()->deleteSession(session);
    if (!result)
    {
        response.setErrno(INTERNAL_DB_ERROR);
        answer.append(response.getJson());
        DEBUG() << "answer: " <<  answer.data();
        return answer;
    }
    m_sessionsContainer->erase(session);
    DEBUG() << "Number of sessions after deleting: " << m_sessionsContainer->size();
#endif

    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    DEBUG() << "answer: " <<  answer.data();
    return answer;
}


QByteArray DbObjectsCollection::processOwnedChannelsQuery(const QByteArray &data)
{
    OwnedChannelsRequestJSON request;
    OwnedChannelsResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

    if (!request.parseJson(data))
    {
        response.setErrno(INCORRECT_JSON_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    Session session =  Core::MetaCache::findSession(request.getSessionToken());
    if(!session.isValid())
    {
        response.setErrno(WRONG_TOKEN_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    BasicUser user = request.getUser();
    QList<Channel> channels = Core::MetaCache::getChannelsByOwner(user);

    response.setChannels(channels);
    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    DEBUG() << "answer: %s" << answer.data();
    return answer;
}

QByteArray DbObjectsCollection::processSubscribedChannelsQuery(const QByteArray &data)
{
    SubscribedChannelsRequestJSON request;
    SubscribedChannelsResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

    if (!request.parseJson(data))
    {
        response.setErrno(INCORRECT_JSON_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    Session session = Core::MetaCache::findSession(request.getSessionToken());
    if(!session.isValid())
    {
        response.setErrno(WRONG_TOKEN_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    QList<Channel> channels = Core::MetaCache::getSubscribedChannels(request.getUser());
    response.setChannels(channels);

    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    DEBUG() << "answer: " << answer.data();
    return answer;
}


////TODO create function that will check validity of authkey, and channel name
QByteArray DbObjectsCollection::processSubscribeQuery(const QByteArray &data)
{
    DEBUG() <<  "starting SubscribeQuery processing";
    SubscribeChannelRequestJSON request;
    SubscribeChannelResponseJSON response;

    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

    if (!request.parseJson(data))
    {
        response.setErrno(INCORRECT_JSON_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    Session session = Core::MetaCache::findSession(request.getSessionToken());

    if(!session.isValid())
    {
        response.setErrno(WRONG_TOKEN_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    common::BasicUser user = session.getUser();

    Channel channel = request.getChannel();
    if(!channel.isValid())
    {
        response.setErrno(CHANNEL_DOES_NOT_EXIST_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    bool result = Core::MetaCache::subscribeChannel(user,channel);
    if(!result)
    {
        response.setErrno(INTERNAL_DB_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    DEBUG() << "answer: " << answer.data();

    return answer;
}

QByteArray DbObjectsCollection::processAddUserQuery(const QByteArray &data)
{
    DEBUG() <<  "starting AddUser processing";
    AddUserRequestJSON request;

    AddUserResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

    if (!request.parseJson(data))
    {
        response.setErrno(INCORRECT_JSON_ERROR);
        answer.append(response.getJson());
        return answer;
    }


    common::BasicUser user = request.getUser();
    if(Core::MetaCache::checkUser(user))
    {
            response.setErrno(USER_ALREADY_EXIST_ERROR);
            answer.append(response.getJson());
            DEBUG() << "answer: " << answer.data();
            return answer;
    }


    if(!Core::MetaCache::addUser(user))
    {
        response.setErrno(INTERNAL_DB_ERROR);
        answer.append(response.getJson());
        DEBUG() << "answer: " << answer.data();
        return answer;
    }

    response.addUser(user);
    response.setErrno(SUCCESS);


    answer.append(response.getJson());
    DEBUG() << "answer: " << answer.data();
    return answer;
}




QByteArray DbObjectsCollection::processUnsubscribeQuery(const QByteArray &data)
{
    UnsubscribeChannelRequestJSON request;
    UnsubscribeChannelResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

    if (!request.parseJson(data))
    {
        response.setErrno(INCORRECT_JSON_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    Session session = Core::MetaCache::findSession(request.getSessionToken());
    if(!session.isValid())
    {
        response.setErrno(WRONG_TOKEN_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    common::BasicUser user = session.getUser();
    bool result = Core::MetaCache::unsubscribeChannel(user,request.getChannel());

    if(!result)
    {
        response.setErrno(CHANNEL_NOT_SUBCRIBED_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    DEBUG() << "answer: " << answer.data();
    return answer;
}


QByteArray DbObjectsCollection::processGetErrnoInfo(const QByteArray&)
{
    ErrnoInfoResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    DEBUG() << "answer: " << answer.data();
    return answer;
}


QByteArray DbObjectsCollection::processBuildQuery(const QByteArray&)
{
    BuildResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

    response.setErrno(SUCCESS);
    response.setVersion(getPlatformBuildInfo());
    answer.append(response.getJson());
    DEBUG() << "answer: " << answer.data();
    return answer;
}

QByteArray DbObjectsCollection::processFilterChannelQuery(const QByteArray& data)
{
    NOT_IMPLEMENTED();
#if 0

    FilterChannelRequestJSON request;
    FilterChannelResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

    if (!request.parseJson(data))
    {
        response.setErrno(INCORRECT_JSON_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    Session dummySession = request.getSessions()->at(0);
    Session realSession = findSession(dummySession);
    if(realSession.isNull())
    {
        response.setErrno(WRONG_TOKEN_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    common::BasicUser realUser = realSession->getUser();
    QList<Channel> channels = realUser->getSubscribedChannels();
    Channel channel;
    for(int i = 0; i<channels->size(); i++)
    {
        if(QString::compare(channels->at(i)->getName(), request.getChannelName(), Qt::CaseInsensitive) == 0)
        {
            channel = channels->at(i);
            break;
        }
    }
    if (channel.isNull())
    {
        response.setErrno(CHANNEL_DOES_NOT_EXIST_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    QList<Tag > tags = m_dataChannelsMap->values(channel);
    int amount = request.getAmount();
    tags = tags.count() > amount ? tags.mid(0, amount) : tags;

    QueryExecutor::instance()->updateSession(realSession);

    response.setData(channel, tags);
    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    DEBUG() << "answer: " << answer.data();
    return answer;
#endif
    return data;
}

QByteArray DbObjectsCollection::processDeleteUserQuery(const QByteArray& data)
{
    DEBUG() <<  "starting DeleteUser processing";
    DeleteUserRequestJSON request;
    DeleteUserResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");
    if (!request.parseJson(data))
    {
        response.setErrno(INCORRECT_JSON_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    common::BasicUser user=request.getUser();

    if(!user.isValid())
    {
        response.setErrno(INCORRECT_CREDENTIALS_ERROR);
        answer.append(response.getJson());
        DEBUG() << "answer: %s" <<  answer.data();
        return answer;
    }

    if(! Core::MetaCache::deleteUser(user))
    {
        response.setErrno(INTERNAL_DB_ERROR);
        answer.append(response.getJson());
        DEBUG() << "answer: " << answer.data();
        return answer;
    }

    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    DEBUG() << "answer: " << answer.data();
    return answer;
}

//QByteArray DbObjectsCollection::processRestorePasswordQuery(const QByteArray& data)
//{
//    DEBUG() <<  "starting RestorePassword processing";
//    RestorePasswordRequestJSON request;
//    RestorePasswordResponseJSON response;
//    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");
//    if (!request.parseJson(data))
//    {
//        response.setErrno(INCORRECT_JSON_ERROR);
//        answer.append(response.getJson());
//        return answer;
//    }
//    // Look for user with the same name
//    Q_ASSERT(false);
//    //common::BasicUser realUser = findUser(request.getUsers()->at(0)->getEmail());

//    //if(!realUser)
//    {
//        response.setErrno(INCORRECT_CREDENTIALS_ERROR);
//        answer.append(response.getJson());
//        DEBUG() << "answer: %s" <<  answer.data();
//        return answer;
//    }

//    //    int passwLength = SettingsStorage::getValue("security/password_length", QVariant(DEFAULT_PASSWORD_LENGTH)).toInt();
//    //    QString password = generateNewPassword(realUser).left(passwLength);
//    //    QString hash = getPasswordHash(realUser->getLogin(), password);
//    //    DEBUG() << realUser->getPassword();
//    //    common::BasicUser updatedUser = QueryExecutor::instance()->updateUserPassword(realUser, hash);

//    //    if(updatedUser.isNull())
//    //    {
//    //        response.setErrno(INTERNAL_DB_ERROR);
//    //        answer.append(response.getJson());
//    //        DEBUG() << "answer: %s" <<  answer.data();
//    //        return answer;
//    //    }

//    //    DEBUG() <<  "Sending email for restoring password";
//    //    EmailMessage message(updatedUser->getEmail());
//    //    message.sendAsRestorePwdMessage(password);

//    //    response.setErrno(SUCCESS);
//    //    answer.append(response.getJson());
//    //    DEBUG() << "answer: %s" <<  answer.data();
//    //    return answer;
//}

void DbObjectsCollection::init()
{

    getInstance();
    Core::MetaCache::init();

}

}                                       // namespace common


/* ===[ End of file ]=== */
