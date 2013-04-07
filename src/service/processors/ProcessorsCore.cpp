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

  // Return true, if credentials are incorrect
  bool DbObjectsCollection::areCredentialsIncorrect(const BasicUser& realUser, const BasicUser& user) const
  {

    bool secirutyEnabled = SettingsStorage::getValue("security/enable",QVariant(true)).toBool();
    bool passwordMatches = (user.getPassword() == realUser.getPassword());

    return !realUser.isValid() || ( realUser.isValid() && !passwordMatches && secirutyEnabled );
  }

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
    common::BasicUser realUser = Core::MetaCache::findUserByName(user.getLogin());
    DEBUG() << "user=" << realUser;


    if( areCredentialsIncorrect(realUser, user) )
    {
	
        response.setErrno(INCORRECT_CREDENTIALS_ERROR);
        DEBUG() << "Incorrect credentilas" ;
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
            Core::MetaCache::updateSession(session);
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

    Core::MetaCache::updateSession(session);

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
    Core::MetaCache::updateSession(session);
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
    Channel realChannel = Core::MetaCache::findChannel(channel.getName());

    if(realChannel.isValid())
    {
        qWarning() << "CHANNEL_ALREADY_EXIST_ERROR";
        response.setErrno(CHANNEL_ALREADY_EXIST_ERROR);
        answer.append(response.getJson());
        return answer;

    }

    if(!Core::MetaCache::addChannel(channel,session.getUser()))
    {
        qWarning() << "INTERNAL_DB_ERROR";
        response.setErrno(INTERNAL_DB_ERROR);
        answer.append(response.getJson());
        DEBUG() << "answer: " << answer.data();
        return answer;
    }

    Core::MetaCache::updateSession(session);

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

    Core::MetaCache::updateSession(session);

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

    response.setErrno(SUCCESS);
    response.setVersion(getPlatformVersion());
    answer.append(response.getJson());
    DEBUG() << "answer: " << answer.data();
    return answer;
}


} // namespace common
