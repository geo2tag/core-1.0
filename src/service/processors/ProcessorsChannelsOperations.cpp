#include "DbObjectsCollection.h"
#include "ErrnoTypes.h"
#include "defines.h"
#include "MetaCache.h"

#include "ChannelsOperationRequestJSON.h"
#include "ChannelsOperationResponseJSON.h"
#include "ChannelsOperator.h"


namespace common
{

QByteArray DbObjectsCollection::processChannelsOperationsQuery(const QByteArray& data)
{
    DEBUG() << "processOperationsQuery";
    ChannelsOperationRequestJSON request;
    ChannelsOperationResponseJSON response;
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

    QList<Channel> channels = request.getChannels();

    if(channels.length() < 2)
    {
        response.setErrno(NOT_SUPPORTED);
        answer.append(response.getJson());
        return answer;
    }

    foreach (Channel channel, channels)
    {
        Channel realChannel = cache->findChannel(channel.getName());
        if(!realChannel.isValid())
        {
            response.setErrno(CHANNEL_DOES_NOT_EXIST_ERROR);
            answer.append(response.getJson());
            return answer;
        }
    }

    ChannelsOperator co(session.getDbName(), request.getFormula());
    response.setTags(co.doOperation());

    response.setErrno(SUCCESS);
    answer.append(response.getJson());
    DEBUG() << "TIME TO DO OPERATION: " << nMilliseconds;
    return answer;
}

} // namespace common
