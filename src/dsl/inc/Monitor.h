#ifndef MONITOR_H
#define MONITOR_H

#include <QString>
#include "MapObject.h"

#define MONITOR_CONDITION_PLACEHOLDER "MONITOR_CONDITION_PLACEHOLDER"

class Monitor: public MapObject
{
public:
    QString name;
    QString condition;

    Monitor(const QString& stringRepresentation);
};

#endif // MONITOR_H
