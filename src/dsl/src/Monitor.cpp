#include "Monitor.h"
#include "ServiceFileParserException.h"

#include <QStringList>
#define MONITOR_PARSING_ERROR "Monitor parsing error!"

Monitor::Monitor(const QString& stringRepresentation)
{
     QStringList items = stringRepresentation.split(",");
     if (items.size() != 2) throw ServiceFileParserException(MONITOR_PARSING_ERROR);

     name = items.at(0).trimmed();
     condition = items.at(1).trimmed();

     addReplacePair(NAME_PLACEHOLDER, name);
     addReplacePair(MONITOR_CONDITION_PLACEHOLDER, condition);
     addReplacePair(BROADCAST_ACTION_PLACEHOLDER, name);
}
