#ifndef SERVICELOGGER_H
#define SERVICELOGGER_H

#include <QObject>

class Log : public QObject
{
    explicit Log();

public:
    static void init();

};

#endif // SERVICELOGGER_H
