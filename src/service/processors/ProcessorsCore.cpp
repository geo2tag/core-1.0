#include "DbObjectsCollection.h"
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

#include "AlterChannelRequestJSON.h"
#include "AlterChannelResponseJSON.h"

#include "ChangePasswordRequestJSON.h"
#include "ChangePasswordResponseJSON.h"

#include "VersionResponseJSON.h"

#include "ErrnoTypes.h"
#include "defines.h"
#include "Session.h"

namespace common
{

  // Return true, if credentials are incorrect
  bool DbObjectsCollection::areCredentialsIncorrect(const BasicUser& user) const
  {
    common::BasicUser realUser = m_defaultCache->findUserByName(user.getLogin());
    DEBUG() << "realUser=" << realUser;
    DEBUG() << "user=" << user;

    bool secirutyEnabled = SettingsStorage::getValue("security/enable",QVariant(true)).toBool();
    bool passwordMatches = (user.getPassword() == realUser.getPassword());

    DEBUG() << "secirutyEnabled = "<< secirutyEnabled << ", passwordMatches = " << passwordMatches;

    if (!realUser.isValid()){
        DEBUG() << "Username is incorrect";
	return true;

    }else {
       if (secirutyEnabled && !passwordMatches){
        DEBUG() << "Security is enabled and password is incorrect";
        DEBUG() << "password from request "<< user.getPassword() << ", real password" << realUser.getPassword();
        return  true;
       }
	
    }
    return false;
    //return !realUser.isValid() || ( realUser.isValid() && !passwordMatches && secirutyEnabled );
  }



QByteArray DbObjectsCollection::processLoginQuery(const QByteArray &data)
{
    LoginRequestJSON request;
    LoginResponseJSON response;
    QByteArray answer;
    answer.append(OK_REQUEST_HEADER);
    if (!request.parseJson(data))
    {
        response.setErrno(INCORRECT_JSON_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    common::BasicUser user = request.getUser();

    if( areCredentialsIncorrect( user) )
    {
	
        response.setErrno(INCORRECT_CREDENTIALS_ERROR);
        DEBUG() << "Incorrect credentilas" ;
    }

    else
    {
        Session session = m_defaultCache->findSession(user);

        if (!session.isValid())
        {
            QString token = Session::generateToken(user);
	    common::BasicUser realUser = m_defaultCache->findUserByName(user.getLogin());
            Session addedSession = Session(token,QDateTime::currentDateTime(), realUser);

            DEBUG() <<  "Session hasn't been found. Generating of new Session.";

            m_defaultCache->insertSession(addedSession);
            response.setSessionToken(token);
        }
        else
        {
            DEBUG() <<  "Session has been found. Session's token:" << session.getSessionToken();
            DEBUG() <<  "Updating session";
            m_defaultCache->updateSession(session);
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

    QByteArray answer(OK_REQUEST_HEADER);

    if (!request.parseJson(data))
    {
        response.setErrno(INCORRECT_JSON_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    Tag tag = request.getTag();


    Session session = m_defaultCache->findSession(request.getSessionToken());

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

    Core::MetaCache * cache = Core::MetaCache::getMetaCache(session);

    if(!cache->testChannel(user,request.getChannel()))
    {
        DEBUG() << "user has no roghts to write";
        response.setErrno(CHANNEL_NOT_SUBCRIBED_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    DEBUG() << "writing tag " << tag;
    if(!cache->writeTag(tag))
    {
        response.setErrno(INTERNAL_DB_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    m_defaultCache->updateSession(session);

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
    QByteArray answer(OK_REQUEST_HEADER);

    if (!request.parseJson(data))
    {
        response.setErrno(INCORRECT_JSON_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    Session session = m_defaultCache->findSession(request.getSessionToken());

    if(!session.isValid())
    {
        response.setErrno(WRONG_TOKEN_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    QList<Tag> feed;
    Core::MetaCache * cache = Core::MetaCache::getMetaCache(session);
    QList<Channel> channels = cache->getSubscribedChannels(session.getUser());

    DEBUG() << "User is subscribed for " << channels.size() << " channels";

    double lat1 = request.getLatitude();
    double lon1 = request.getLongitude();
    double radius  = request.getRadius();

    Channel channel;
    foreach(channel, channels)
    {
        QList<Tag> tags = cache->loadTagsFromChannel(channel);
        Tag tag;
        foreach(tag,tags)
        {
            if(Tag::getDistance(lat1, lon1, tag.getLatitude(), tag.getLongitude())<radius )
            feed.push_back(tag);
        }
    }
    qSort(feed);
    response.setData(feed);

    DEBUG() << "Updating session";
    m_defaultCache->updateSession(session);
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
    QByteArray answer(OK_REQUEST_HEADER);

    if (!request.parseJson(data))
    {
        qWarning() << "INCORRECT_JSON_ERROR";
        response.setErrno(INCORRECT_JSON_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    Session session = m_defaultCache->findSession(request.getSessionToken());
    DEBUG() << "Session:" << session;

    if(!session.isValid())
    {
        qWarning() << "WRONG_TOKEN_ERROR";
        response.setErrno(WRONG_TOKEN_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    Core::MetaCache * cache = Core::MetaCache::getMetaCache(session);
    Channel channel = request.getChannel();
    Channel realChannel = cache->findChannel(channel.getName());

    if(realChannel.isValid())
    {
        qWarning() << "CHANNEL_ALREADY_EXIST_ERROR";
        response.setErrno(CHANNEL_ALREADY_EXIST_ERROR);
        answer.append(response.getJson());
        return answer;

    }

    if(!cache->addChannel(channel,session.getUser()))
    {
        qWarning() << "INTERNAL_DB_ERROR";
        response.setErrno(INTERNAL_DB_ERROR);
        answer.append(response.getJson());
        DEBUG() << "answer: " << answer.data();
        return answer;
    }

    m_defaultCache->updateSession(session);

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
    QByteArray answer(OK_REQUEST_HEADER);

    if (!request.parseJson(data))
    {
        response.setErrno(INCORRECT_JSON_ERROR);
        answer.append(response.getJson());
        return answer;
    }
    Session session = m_defaultCache->findSession(request.getSessionToken());
    if(!session.isValid())
    {
        response.setErrno(WRONG_TOKEN_ERROR);
        answer.append(response.getJson());
        return answer;
    }


    m_defaultCache->updateSession(session);
    Core::MetaCache * cache = Core::MetaCache::getMetaCache(session);

    response.setChannels(cache->getChannels());
    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    DEBUG() << "answer: " << answer.data();
    return answer;
}

QByteArray DbObjectsCollection::processAlterChannelQuery(const QByteArray &data)
{
    AlterChannelRequestJSON request;
    AlterChannelResponseJSON response;
    DEBUG() << "processAlterChannelQuery - data = " << data.data();
    QByteArray answer(OK_REQUEST_HEADER);

    if (!request.parseJson(data))
    {
        response.setErrno(INCORRECT_JSON_ERROR);
        answer.append(response.getJson());
        return answer;
    }
    Session session = m_defaultCache->findSession(request.getSessionToken());
    if(!session.isValid())
    {
        response.setErrno(WRONG_TOKEN_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    Core::MetaCache * cache = Core::MetaCache::getMetaCache(session);

    Channel channel = cache->findChannel(request.getName());
    if (!channel.isValid())
    {
        response.setErrno(CHANNEL_DOES_NOT_EXIST_ERROR);
        answer.append(response.getJson());
        return answer;
    }	

    bool isOwned = cache->isOwner(session, request.getName());
    if (!isOwned)
    {
        response.setErrno(USER_DOES_NOT_OWN_CHANNEL_ERROR);
        answer.append(response.getJson());
        return answer;

    }

    m_defaultCache->updateSession(session);
    bool result = cache->alterChannel(request.getName(), request.getField(), request.getValue());
    if (!result){
	response.setErrno(INTERNAL_DB_ERROR);
    }else{
	response.setErrno(SUCCESS);
    }
    answer.append(response.getJson());
    DEBUG() << "answer: " << answer.data();
    return answer;
}

QByteArray DbObjectsCollection::processVersionQuery(const QByteArray&)
{
    VersionResponseJSON response;
    QByteArray answer(OK_REQUEST_HEADER);

    response.setErrno(SUCCESS);
    response.setVersion(getPlatformVersion());
    answer.append(response.getJson());
    DEBUG() << "answer: " << answer.data();
    return answer;
}

QByteArray DbObjectsCollection::processChangePasswordQuery(const QByteArray &data)
{
    ChangePasswordRequestJSON request;
    ChangePasswordResponseJSON response;
    QByteArray answer;
    answer.append(OK_REQUEST_HEADER);
    if (!request.parseJson(data))
    {
      response.setErrno(INCORRECT_JSON_ERROR);
      answer.append(response.getJson());
      return answer;
    }

    common::BasicUser user = request.getUser();
    if (areCredentialsIncorrect(user))
    {
      response.setErrno(INCORRECT_CREDENTIALS_ERROR);
      DEBUG() << "Incorrect credentilas";
    }else{
	// Do password changing	
      QString newPassword = request.getNewPassword();
      bool changePasswordResult = m_defaultCache->changePassword(user.getLogin(), newPassword);
      
      if (changePasswordResult)
        response.setErrno(SUCCESS);
      else 
        response.setErrno(INTERNAL_DB_ERROR); 
    }

    answer.append(response.getJson());
    DEBUG() << "answer: " << answer.data();
    return answer;
}


} // namespace common
