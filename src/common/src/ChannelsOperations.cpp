#include "ChannelsOperations.h"

ChannelsOperations::ChannelsOperations(const Channel& channel_1, const Channel& channel_2)
{
    this->channel_1 = channel_1;
    this->channel_2 = channel_2;
}

QList<Tag> ChannelsOperations::doOperation(QString type)
{
    if (type == "intersection")
        return doIntersection();
    else if (type == "union")
        return doUnion();
    else
        return doComplement();
}

QList<Tag> ChannelsOperations::doIntersection()
{
    QList<Tag> tags;
    return tags;
}

QList<Tag> ChannelsOperations::doUnion()
{
    QList<Tag> tags;
    return tags;
}

QList<Tag> ChannelsOperations::doComplement()
{
    QList<Tag> tags;
    return tags;
}
