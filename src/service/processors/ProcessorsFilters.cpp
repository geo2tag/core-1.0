#include "DbObjectsCollection.h"
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

#include "MetaCache.h"

namespace common
{

QByteArray DbObjectsCollection::processFilterCircleQuery(const QByteArray& data)
{
    DEBUG() <<  ">>> processFilterCircleQuery";

    FilterCircleRequestJSON request;
    return internalProcessFilterQuery(request, data, false);
}

QByteArray DbObjectsCollection::processFilterCylinderQuery(const QByteArray& data)
{
    FilterCylinderRequestJSON request;
    return internalProcessFilterQuery(request, data, true);
}

QByteArray DbObjectsCollection::processFilterPolygonQuery(const QByteArray& data)
{
    FilterPolygonRequestJSON request;
    return internalProcessFilterQuery(request, data, false);
}

QByteArray DbObjectsCollection::processFilterRectangleQuery(const QByteArray& data)
{
    FilterRectangleRequestJSON request;
    return internalProcessFilterQuery(request, data, false);
}

QByteArray DbObjectsCollection::processFilterBoxQuery(const QByteArray& data)
{
    FilterBoxRequestJSON request;
    return internalProcessFilterQuery(request, data, true);
}

QByteArray DbObjectsCollection::processFilterFenceQuery(const QByteArray& data)
{
    FilterFenceRequestJSON request;
    return internalProcessFilterQuery(request, data, true);
}

QByteArray DbObjectsCollection::internalProcessFilterQuery(FilterRequestJSON& request,
                                                           const QByteArray& data, bool is3d)
{
    Filtration filtration;
    FilterDefaultResponseJSON response;
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

    filtration.addFilter(QSharedPointer<Filter>(new ShapeFilter(request.getShape())));
    filtration.addFilter(QSharedPointer<Filter>(new TimeFilter(request.getTimeFrom(), request.getTimeTo())));
    if(is3d)
    {
        filtration.addFilter(QSharedPointer<Filter>(new AltitudeFilter(request.getAltitude1(), request.getAltitude2())));
    }

    QList<Channel> channels = Core::MetaCache::getSubscribedChannels(user);
    QList<Tag> feed;

    if (request.getChannels().size() > 0)
    {
        Channel targetChannel;
        // look for ...
        for(int i = 0; i<channels.size(); i++)
        {
            if (channels.at(i).getName() == request.getChannels().at(0).getName())
            {
                targetChannel = channels.at(i);
                break;
            }
        }

        if (!targetChannel.isValid())
        {
            response.setErrno(CHANNEL_DOES_NOT_EXIST_ERROR);
            answer.append(response.getJson());
            DEBUG() << "answer: " << answer.data();
            return answer;
        }

        QList<Tag > tags = Core::MetaCache::loadTagsFromChannel(targetChannel);
        QList<Tag > filteredTags = filtration.filtrate(tags);

        feed << filteredTags;
        response.setErrno(SUCCESS);
    }
    else
    {
        for(int i = 0; i<channels.size(); i++)
        {
            Channel channel = channels.at(i);
            QList<Tag > tags = Core::MetaCache::loadTagsFromChannel(channel);
            QList<Tag > filteredTags = filtration.filtrate(tags);
                feed << filteredTags;
        }
	DEBUG() << "Filtred tags number " << feed.size();

	Core::MetaCache::updateSession(session);

        response.setErrno(SUCCESS);
    }
    response.setTags(feed);

    answer.append(response.getJson());
    DEBUG() << "answer: " << answer.data();
    return answer;
}

} // namespace common
