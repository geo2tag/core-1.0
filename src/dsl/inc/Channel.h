#ifndef CHANNEL_H
#define CHANNEL_H

#include <QString>

class Channel
{

public:
    QString trackable;
    QString name;
    QString tagNumber;

    Channel(const QString& stringRepresentation);
};

#endif // CHANNEL_H
