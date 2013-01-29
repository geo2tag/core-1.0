#ifndef SERVICELOGGER_H
#define SERVICELOGGER_H

#include <QObject>
#include <QDebug>

class Log : public QObject
{
    explicit Log();

public:
    static void init();

};

#define DEBUG() ( qDebug() <<__FILE__<<":"<<__LINE__ )
#define WARNING() ( qWarning() <<__FILE__<<":"<<__LINE__ )

#endif // SERVICELOGGER_H
