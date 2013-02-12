#include "DbSession.h"
#include "SettingsStorage.h"
#include "MetaCache.h"

#include "LoginRequestJSON.h"
#include "LoginResponseJSON.h"

#include "WriteTagRequestJSON.h"
#include "WriteTagResponseJSON.h"

#include "LoadTagsRequestJSON.h"
#include "LoadTagsResponseJSON.h"

#include "AvailableChannelsRequestJSON.h"
#include "AvailableChannelsResponseJSON.h"

#include "AddChannelRequestJSON.h"
#include "AddChannelResponseJSON.h"

#include "VersionResponseJSON.h"

#include "ErrnoTypes.h"
#include "defines.h"
#include "Session.h"

namespace common
{

QByteArray DbObjectsCollection::processLoginQuery(const QByteArray &data)
{
    LoginRequestJSON request;
    LoginResponseJSON response;
    QByteArray answer;
    answer.append("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");
    if (!request.parseJson(data))
    {
        response.setErrno(INCORRECT_JSON_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    common::BasicUser user = request.getUser();
    DEBUG() << "user=" << user;

    bool secirutyEnabled = SettingsStorage::getValue("security/enable",QVariant(true)).toBool();

    if(user.isValid() && secirutyEnabled)
    {
        response.setErrno(INCORRECT_CREDENTIALS_ERROR);
        DEBUG() << "Incorrect credentilas, security/enabled=" << secirutyEnabled;
    }

    else
    {
        Session session = Core::MetaCache::findSession(user);

        if (!session.isValid())
        {
            QString token=Session::generateToken(user);
            Session addedSession = Session(token,QDateTime::currentDateTime(),user);

            DEBUG() <<  "Session hasn't been found. Generating of new Session.";

            Core::MetaCache::insertSession(addedSession);
            response.setSessionToken(token);
        }
        else
        {
            DEBUG() <<  "Session has been found. Session's token:" << session.getSessionToken();
            DEBUG() <<  "Updating session";
            QueryExecutor::instance()->updateSession(session);
            response.setSessionToken(session.getSessionToken());
        }
        response.setErrno(SUCCESS);
    }
    answer.append(response.getJson());
    DEBUG() << "answer: " <<  answer.data();
    return answer;
}


QByteArray DbObjectsCollection::processWriteTagQuery(const QByteArray &data)
{
    WriteTagRequestJSON request;
    WriteTagResponseJSON response;

    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

    if (!request.parseJson(data))
    {
        response.setErrno(INCORRECT_JSON_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    Tag tag = request.getTag();


    Session session = Core::MetaCache::findSession(request.getSessionToken());

    DEBUG() << "Checking for sessions with token = " << session.getSessionToken();
    DEBUG() << "Session:" << session;

    if(!session.isValid())
    {
        response.setErrno(WRONG_TOKEN_ERROR);
        answer.append(response.getJson());
        DEBUG() << "invalid session";
        return answer;
    }

    common::BasicUser user = session.getUser();
    tag.setUser(user);

    if(!Core::MetaCache::testChannel(user,request.getChannel()))
    {
        DEBUG() << "user has no roghts to write";
        response.setErrno(CHANNEL_NOT_SUBCRIBED_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    DEBUG() << "writing tag " << tag;
    if(!Core::MetaCache::writeTag(tag))
    {
        response.setErrno(INTERNAL_DB_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    response.setErrno(SUCCESS);
    response.addTag(tag);

    answer.append(response.getJson());
    DEBUG() <<  "answer: " << answer.data();
    return answer;
}

QByteArray DbObjectsCollection::processLoadTagsQuery(const QByteArray &data)
{
    LoadTagsRequestJSON request;
    LoadTagsResponseJSON response;
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

    QList<Tag> feed;

    QList<Channel> channels = Core::MetaCache::getSubscribedChannels(session.getUser());

    DEBUG() << "User is subscribed for " << channels.size() << " channels";

    double lat1 = request.getLatitude();
    double lon1 = request.getLongitude();
    double radius  = request.getRadius();

    Channel channel;
    foreach(channel, channels)
    {
        QList<Tag> tags = Core::MetaCache::loadTagsFromChannel(channel);
        Tag tag;
        foreach(tag,tags)
        {
            if(Tag::getDistance(lat1, lon1, tag.getLatitude(), tag.getLongitude())<radius )
            feed.push_back(tag);
        }
    }

    response.setData(feed);

    DEBUG() << "Updating session";
    QueryExecutor::instance()->updateSession(session);
    DEBUG() << "Updating session ..done";

    response.setErrno(SUCCESS);

    answer.append(response.getJson());

    DEBUG() << "answer: " << answer.data();
    return answer;
}

QByteArray DbObjectsCollection::processAddChannelQuery(const QByteArray &data)
{
    DEBUG() <<  "starting AddChannelQuery processing";
    AddChannelRequestJSON request;
    AddChannelResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

    if (!request.parseJson(data))
    {
        qWarning() << "INCORRECT_JSON_ERROR";
        response.setErrno(INCORRECT_JSON_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    Session session = Core::MetaCache::findSession(request.getSessionToken());
    DEBUG() << "Session:" << session;

    if(!session.isValid())
    {
        qWarning() << "WRONG_TOKEN_ERROR";
        response.setErrno(WRONG_TOKEN_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    Channel channel = request.getChannel();

    if(!QueryExecutor::instance()->insertNewChannel(channel,session.getUser()))
    {
        qWarning() << "INTERNAL_DB_ERROR";
        response.setErrno(INTERNAL_DB_ERROR);
        answer.append(response.getJson());
        DEBUG() << "answer: " << answer.data();
        return answer;
    }

    QueryExecutor::instance()->updateSession(session);

    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    DEBUG() << "answer: " << answer.data();
    return answer;
}


QByteArray DbObjectsCollection::processAvailableChannelsQuery(const QByteArray &data)
{
    AvailableChannelsRequestJSON request;
    AvailableChannelsResponseJSON response;
    DEBUG() << "processAvailableChannelsQuery - data = " << data.data();
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

    QueryExecutor::instance()->updateSession(session);

    response.setChannels(Core::MetaCache::getChannels());
    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    DEBUG() << "answer: " << answer.data();
    return answer;
}

QByteArray DbObjectsCollection::processVersionQuery(const QByteArray&)
{
    VersionResponseJSON response;
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

    QString version = SettingsStorage::getValue("general/geo2tag_version").toString();

    response.setErrno(SUCCESS);
    response.setVersion(version);
    answer.append(response.getJson());
    DEBUG() << "answer: " << answer.data();
    return answer;
}


} // namespace common
