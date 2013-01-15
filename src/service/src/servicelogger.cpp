#include "servicelogger.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

#include <syslog.h>

static const char *file="/var/log/geo2tag.log";

static void handler(QtMsgType type, const char *msg)
{
    static QFile output(file);

    if(!output.isOpen())
    {
        if(!output.open(QIODevice::ReadWrite|QIODevice::Append))
        {
            //this is only place for syslog
            syslog(LOG_ERR,"Geo2tag service ERROR, cannot open log file %s",file);
        }
        QTextStream beginlog(&output);
        beginlog << "=== BEGIN LOG at " << QDateTime::currentDateTimeUtc().toString(Qt::ISODate) << "===" << endl;
    }

    QTextStream stream(&output);
    stream << QDateTime::currentDateTimeUtc().toString(Qt::ISODate);

    switch (type)
    {
    case QtDebugMsg:
        stream << " [DBG]: " << msg << endl;
        break;
    case QtWarningMsg:
        stream << " [WRN]: " << msg << endl;
        break;
    case QtCriticalMsg:
        stream << " [CRT]: " << msg << endl;
        break;
    case QtFatalMsg:
        stream << " [FTL]: " << msg << endl;
    }
}

void Log::init()
{
    qInstallMsgHandler(handler);
}
