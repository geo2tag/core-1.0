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
    qDebug() << "user=" << user;

    bool secirutyEnabled = SettingsStorage::getValue("security/enable",QVariant(true)).toBool();

    if(user.isValid() && secirutyEnabled)
    {
        response.setErrno(INCORRECT_CREDENTIALS_ERROR);
        qDebug() << "Incorrect credentilas, security/enabled=" << secirutyEnabled;
    }

    else
    {
        Session session = Core::MetaCache::findSession(user);
        if (!session.isValid())
        {
            qDebug() <<  "Session hasn't been found. Generating of new Session.";
            Session addedSession = QueryExecutor::instance()->insertNewSession(user);
            if (!addedSession.isValid())
            {
                response.setErrno(INTERNAL_DB_ERROR);
                answer.append(response.getJson());
                qDebug() << "answer: " <<  answer.data();
                return answer;
            }

            Core::MetaCache::reloadSessions();
            response.addSession(addedSession);
        }
        else
        {
            qDebug() <<  "Session has been found. Session's token:" << session.getSessionToken();
            qDebug() <<  "Updating session";
            QueryExecutor::instance()->updateSession(session);
            response.addSession(session);
        }
        response.setErrno(SUCCESS);
    }
    answer.append(response.getJson());
    qDebug() << "answer: " <<  answer.data();
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

    Session session = request.getSession();
    qDebug() << "Checking for sessions with token = " << session.getSessionToken();

    if(session.isValid())
    {
        response.setErrno(WRONG_TOKEN_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    common::BasicUser user = session.getUser();
    if(!Core::MetaCache::testChannel(user,request.getChannel()))
    {
        qDebug() << "user has no roghts to write";
        response.setErrno(CHANNEL_NOT_SUBCRIBED_ERROR);
        answer.append(response.getJson());
        return answer;
    }


    if(!Core::MetaCache::writeTag(tag))
    {
        response.setErrno(INTERNAL_DB_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    qDebug() << "Updating session";
    QueryExecutor::instance()->updateSession(session);
    qDebug() << "Updating session ..done";

    response.setErrno(SUCCESS);
    response.addTag(tag);

    answer.append(response.getJson());
    qDebug() <<  "answer: " << answer.data();
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

    Session session = request.getSession();
    if(!session.isValid())
    {
        response.setErrno(WRONG_TOKEN_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    QList<Channel> channels = QueryExecutor::instance()->getChannelsByUser(session.getUser());
    QList<Tag> feed;

    double lat1 = request.getLatitude();
    double lon1 = request.getLongitude();
    double radius  = request.getRadius();

    Channel channel;
    foreach(channel, channels)
    {
        QList<Tag> tags = QueryExecutor::instance()->loadTags(channel);
        Tag tag;
        foreach(tag,tags)
        {
            if(Tag::getDistance(lat1, lon1, tag.getLatitude(), tag.getLongitude())<radius )
            feed.push_back(tag);
        }
    }


//    for(int i = 0; i<channels->size(); i++)
//    {
//        QSharedPointer<Channel> channel = channels->at(i);
//        QList<QSharedPointer<Tag> > tags = m_dataChannelsMap->values(channel);
//        qSort(tags);
//        for(int j = 0; j < tags.size(); j++)
//        {
//            QSharedPointer<Tag> mark = tags.at(j);
//            double lat2 = mark->getLatitude();
//            double lon2 = mark->getLongitude();

//            if ( Tag::getDistance(lat1, lon1, lat2, lon2) < radius )
//                feed.insert(channel, mark);
//        }
//    }

    response.setData(feed);

    qDebug() << "Updating session";
    QueryExecutor::instance()->updateSession(session);
    qDebug() << "Updating session ..done";

    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    qDebug() << "answer: " << answer.data();
    return answer;
}

QByteArray DbObjectsCollection::processAddChannelQuery(const QByteArray &data)
{
    qDebug() <<  "starting AddChannelQuery processing";
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

    Session session = request.getSession();
    if(!session.isValid())
    {
        qWarning() << "WRONG_TOKEN_ERROR";
        response.setErrno(WRONG_TOKEN_ERROR);
        answer.append(response.getJson());
        return answer;
    }

    Channel channel = request.getChannel();

    if(!QueryExecutor::instance()->insertNewChannel(channel))
    {
        qWarning() << "INTERNAL_DB_ERROR";
        response.setErrno(INTERNAL_DB_ERROR);
        answer.append(response.getJson());
        qDebug() << "answer: " << answer.data();
        return answer;
    }

    QueryExecutor::instance()->updateSession(session);

    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    qDebug() << "answer: " << answer.data();
    return answer;
}


QByteArray DbObjectsCollection::processAvailableChannelsQuery(const QByteArray &data)
{
    AvailableChannelsRequestJSON request;
    AvailableChannelsResponseJSON response;
    qDebug() << "processAvailableChannelsQuery - data = " << data.data();
    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

    if (!request.parseJson(data))
    {
        response.setErrno(INCORRECT_JSON_ERROR);
        answer.append(response.getJson());
        return answer;
    }
    Session session = request.getSession();
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
    qDebug() << "answer: " << answer.data();
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
    qDebug() << "answer: " << answer.data();
    return answer;
}


} // namespace common
