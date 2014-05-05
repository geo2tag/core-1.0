#include "Trackable.h"
#include "ServiceFileParserException.h"
#include <QDebug>
#include <QStringList>


Trackable::Trackable(const QString &stringRepresentation)
{
    QStringList items = stringRepresentation.split(",");
    if (items.size() != 2) return;

    name = items.at(0).trimmed();
    isOnline = IsOnlineEnum(items.at(1).trimmed());

    addReplacePair(NAME_PLACEHOLDER, name);

    QString isOnlineString;

    if (isOnline.value == ONLINE)
         isOnlineString = "true";
    else isOnlineString = "false";

    addReplacePair(IS_ONLINE_PLACEHOLDER, isOnlineString);

}

void Trackable::setChannel(const QString& channel)
{
    addReplacePair(CHANNEL_NAME_PLACEHOLDER, channel);
}
