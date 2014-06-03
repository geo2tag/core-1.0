#include "ChannelsOperator.h"
#include <QSqlRecord>
#include <QSqlError>
#include <servicelogger.h>


ChannelsOperator::ChannelsOperator(const QString& dbName, const QStringList& formula):QueryExecutor(dbName)
{
    this->formula = formula;
}


void ChannelsOperator::retrieveTagsToList(QList<Tag>& container, QSqlQuery& query)
{
    while (query.next())
    {

        QDateTime time = query.record().value("time").toDateTime().toTimeSpec(Qt::LocalTime);
        double latitude = query.record().value("latitude").toDouble();
        double altitude = query.record().value("altitude").toDouble();
        double longitude = query.record().value("longitude").toDouble();
        QString label = query.record().value("label").toString();
        QString description = query.record().value("description").toString();
        QString url = query.record().value("url").toString();

        qlonglong userId = query.record().value("user_id").toLongLong();
        common::BasicUser user = getUserById(userId);


        Tag tag(altitude,latitude,longitude,label,description,url,time);
        tag.setUser(user);

        container.push_back(tag);
    }

}


QList<Tag> ChannelsOperator::doOperation()
{
    QList<Tag> tags;
    QSqlQuery query=makeQuery();
    QString queryString = "";
    foreach (QString token, formula) {
       if (token == "(" || token == ")") {
           queryString.append(token);
       } else if(token == "|") {
           queryString.append(" UNION ");
       } else if (token == "&") {
           queryString.append(" INTERSECT ");
       } else if (token == "\\") {
           queryString.append(" EXCEPT ");
       } else {
            queryString.append("select time, altitude, latitude, longitude, label, description, url, user_id "
                               "from tag where channel_id = " + QString::number(getChannelIdByName(token)));
       }
    }


    queryString.simplified();
    queryString.append(";");

    DEBUG() << "Executing: " << queryString;

    query.exec(queryString);
    DEBUG() << query.lastError();

    retrieveTagsToList(tags, query);

    return tags;
}
