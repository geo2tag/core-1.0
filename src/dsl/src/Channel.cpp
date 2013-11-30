#include "Channel.h"
#include "ServiceFileParserException.h"

#include <QStringList>

#define CHANNEL_PARSING_ERROR "Channel parsing error!"


Channel::Channel(const QString &stringRepresentation)
{
    QStringList items = stringRepresentation.split(",");
    if (items.size() != 3) throw ServiceFileParserException(CHANNEL_PARSING_ERROR);

    trackable = items.at(0);
    name = items.at(1);
    tagNumber = items.at(2);
}
