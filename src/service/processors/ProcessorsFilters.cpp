#include "DbSession.h"
#include "ErrnoTypes.h"
#include "defines.h"
#include "SettingsStorage.h"


#include "Filtration.h"
#include "Filter.h"
#include "TimeFilter.h"
#include "ShapeFilter.h"
#include "AltitudeFilter.h"

#include "FilterDefaultResponseJSON.h"
#include "FilterCircleRequestJSON.h"
#include "FilterCylinderRequestJSON.h"
#include "FilterPolygonRequestJSON.h"
#include "FilterRectangleRequestJSON.h"
#include "FilterBoxRequestJSON.h"
#include "FilterFenceRequestJSON.h"

#include "FilterChannelRequestJSON.h"
#include "FilterChannelResponseJSON.h"


namespace common
{

QByteArray DbObjectsCollection::processFilterCircleQuery(const QByteArray& data)
{
    return data;
//    qDebug() <<  ">>> processFilterCircleQuery";

//    FilterCircleRequestJSON request;
//    return internalProcessFilterQuery(request, data, false);
}

QByteArray DbObjectsCollection::processFilterCylinderQuery(const QByteArray& data)
{
    return data;
//    FilterCylinderRequestJSON request;
//    return internalProcessFilterQuery(request, data, true);
}

QByteArray DbObjectsCollection::processFilterPolygonQuery(const QByteArray& data)
{
    return data;

//    FilterPolygonRequestJSON request;
//    return internalProcessFilterQuery(request, data, false);
}

QByteArray DbObjectsCollection::processFilterRectangleQuery(const QByteArray& data)
{
    return data;
//    FilterRectangleRequestJSON request;
//    return internalProcessFilterQuery(request, data, false);
}

QByteArray DbObjectsCollection::processFilterBoxQuery(const QByteArray& data)
{
    return data;
//    FilterBoxRequestJSON request;
//    return internalProcessFilterQuery(request, data, true);
}

QByteArray DbObjectsCollection::processFilterFenceQuery(const QByteArray& data)
{
    return data;
//    FilterFenceRequestJSON request;
//    return internalProcessFilterQuery(request, data, true);
}

QByteArray DbObjectsCollection::internalProcessFilterQuery(FilterRequestJSON& /*request*/,
                                                           const QByteArray& data, bool /*is3d*/)
{
    return data;

//    Filtration filtration;
//    FilterDefaultResponseJSON response;
//    QByteArray answer("Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");

//    if (!request.parseJson(data))
//    {
//        response.setErrno(INCORRECT_JSON_ERROR);
//        answer.append(response.getJson());
//        return answer;
//    }

//    QSharedPointer<Session> dummySession = request.getSessions()->at(0);
//    QSharedPointer<Session> realSession = findSession(dummySession);
//    if(realSession.isNull())
//    {
//        response.setErrno(WRONG_TOKEN_ERROR);
//        answer.append(response.getJson());
//        return answer;
//    }

//    QSharedPointer<User> realUser = realSession->getUser();

//    filtration.addFilter(QSharedPointer<Filter>(new ShapeFilter(request.getShape())));
//    filtration.addFilter(QSharedPointer<Filter>(new TimeFilter(request.getTimeFrom(), request.getTimeTo())));
//    if(is3d)
//    {
//        filtration.addFilter(QSharedPointer<Filter>(new AltitudeFilter(request.getAltitude1(), request.getAltitude2())));
//    }

//    QSharedPointer<Channels> channels = realUser->getSubscribedChannels();
//    DataChannels feed;
//    if (request.getChannels()->size() > 0)
//    {
//        qDebug() <<  "point_2";

//        QSharedPointer<Channel> targetChannel;
//        // look for ...
//        for(int i = 0; i<channels->size(); i++)
//        {
//            if (channels->at(i)->getName() == request.getChannels()->at(0)->getName())
//            {
//                targetChannel = channels->at(i);
//                break;
//            }
//        }

//        if (targetChannel.isNull())
//        {
//            response.setErrno(CHANNEL_DOES_NOT_EXIST_ERROR);
//            answer.append(response.getJson());
//            qDebug() << "answer: " << answer.data();
//            return answer;
//        }
//        QList<QSharedPointer<Tag> > tags = m_dataChannelsMap->values(targetChannel);
//        QList<QSharedPointer<Tag> > filteredTags = filtration.filtrate(tags);
//        for(int i = 0; i < filteredTags.size(); i++)
//        {
//            feed.insert(targetChannel, filteredTags.at(i));
//        }
//        response.setErrno(SUCCESS);
//    }
//    else
//    {
//        for(int i = 0; i<channels->size(); i++)
//        {
//            QSharedPointer<Channel> channel = channels->at(i);
//            QList<QSharedPointer<Tag> > tags = m_dataChannelsMap->values(channel);
//            QList<QSharedPointer<Tag> > filteredTags = filtration.filtrate(tags);
//            for(int j = 0; j < filteredTags.size(); j++)
//            {
//                feed.insert(channel, filteredTags.at(j));
//            }
//        }
//        response.setErrno(SUCCESS);
//    }
//    response.setDataChannels(feed);

//    QueryExecutor::instance()->updateSession(realSession);

//    answer.append(response.getJson());
//    qDebug() << "answer: " << answer.data();
//    return answer;
}

} // namespace common
