#ifndef CHANNELSOPERATIONS_H
#define CHANNELSOPERATIONS_H

#include <QString>
#include <QList>
#include <DataMarks.h>
#include <Channel.h>

class ChannelsOperations
{
private:
    Channel channel_1;
    Channel channel_2;

public:
    ChannelsOperations(const Channel&, const Channel&);
    QList<Tag> doOperation(QString);
    QList<Tag> doIntersection();
    QList<Tag> doUnion();
    QList<Tag> doComplement();
};

#endif // CHANNELSOPERATIONS_H
