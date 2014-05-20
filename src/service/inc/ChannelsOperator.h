#ifndef CHANNELSOPERATOR_H
#define CHANNELSOPERATOR_H

#include <QString>
#include <QList>
#include <QSqlQuery>
#include <QStringList>
#include <DataMarks.h>
#include <Channel.h>
#include <QueryExecutor.h>

class ChannelsOperator: public QueryExecutor
{
private:
    QStringList formula;

public:
    ChannelsOperator(const QString&, const QStringList&);
    QList<Tag> doOperation();
    void retrieveTagsToList(QList<Tag>& container, QSqlQuery& query);
};

#endif // CHANNELSOPERATOR_H
