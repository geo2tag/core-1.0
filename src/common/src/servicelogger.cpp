#include "servicelogger.h"
#include <QFile>
#include <QDateTime>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QSharedPointer>
#include "SettingsStorage.h"

#include <syslog.h>


static const char *file="/var/log/geo2tag.log";
QString NLog::severity_name[Log::severity_last_element__];
QString NLog::facility_name[Log::facility_last_element__];
bool NLog::log_enabled[Log::facility_last_element__][Log::severity_last_element__];
bool NLog::is_init=false;
using namespace NLog;
static void handler(QtMsgType type, const char *msg);

void write_to_log(const char *msg, Log::Severity sev, Log::Facility fac)
{
    if(log_enabled[fac][sev])
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
        stream<<" ["<<facility_name[fac]<<" : "<<severity_name[sev]<<"] ";
        stream<<msg<<endl;
    }
    //Terminate the application if FATAL() macro was called
    if(fac!=Log::undefined_fac&&sev==Log::fatal_sev)
    {
        qInstallMsgHandler(0);
        qFatal(" ");
    }
}
MessageAssembler:: MessageAssembler(Log::Severity _msg_sev,Log::Facility _msg_fac):msg_sev(_msg_sev),msg_fac(_msg_fac),stream(&contents)
{
}
MessageAssembler::~MessageAssembler()
{
}
//yasper::ptr<MessageAssembler> MessageAssembler::get_instance(Log::Severity _msg_sev,Log::Facility _msg_fac)
QSharedPointer<MessageAssembler> MessageAssembler::get_instance(Log::Severity _msg_sev,Log::Facility _msg_fac)
{
    QSharedPointer<MessageAssembler> res=QSharedPointer<MessageAssembler> (new MessageAssembler(_msg_sev,_msg_fac));
    return res;
}
MessageAssembler NLog::assembler(Log::severity_last_element__,Log::facility_last_element__);
void MessageAssembler::operator+=(MessageAssembler& result)
{
    write_to_log(result.contents.toStdString().c_str(),result.msg_sev,result.msg_fac);
}

//Captures messages from qWarning(), qDebug(), qCritical(), qFatal()
static void handler(QtMsgType type, const char *msg)
{
    Log::Severity sev;
    switch (type)
    {
    case QtDebugMsg:
        sev=Log::debug_sev;
        break;
    case QtWarningMsg:
        sev=Log::warning_sev;
        break;
    case QtCriticalMsg:
        sev=Log::critical_sev;
        break;
    case QtFatalMsg:
        sev=Log::fatal_sev;
    }
    write_to_log(msg,sev,Log::undefined_fac);
}
void Log::init()
{
    if(is_init)
        return;
    severity_name[Log::debug_sev]="debug";
    severity_name[Log::warning_sev]="warning";
    severity_name[Log::critical_sev]="critical";
    severity_name[Log::fatal_sev]="fatal";

    facility_name[Log::undefined_fac]="undefined";
    facility_name[Log::test_fac]="test";
    facility_name[Log::test2_fac]="Flying Spagetti Monster";

    /*By default all log messages are enabled. Severity mode x specified
     *for facility y, means that log with facility y and severity y is
     *enabled. "none" specified for a facility will disable all messages from it
     */
    QString default_log_mode("");
    for(int i=0;i<Log::severity_last_element__;++i)
    {
        default_log_mode+=severity_name[i];
        if(i+1!=Log::severity_last_element__)
            default_log_mode+=",";
    }
    for(int i=0;i<Log::facility_last_element__;++i)
    {
        QString formatted_facility_name=facility_name[i].simplified();
        formatted_facility_name.replace(" ","");
        QVariant saved_data=SettingsStorage::getValue("Log_Settings/"+formatted_facility_name,QVariant(default_log_mode.split(",")));

        QList<QVariant> list=saved_data.toList();
        //QVariant doesn't convert a single string to list
        if(list.empty())
            list.append(saved_data);

        for(QList<QVariant>::iterator j=list.begin();j!=list.end();++j)
        {
            QString st=j->toString();
            if(st=="none")
                break;
            for(int k=0;k<Log::severity_last_element__;++k)
            {
                if(severity_name[k]==st)
                {
                    log_enabled[i][k]=true;
                    break;
                }
            }
        }
    }
    qInstallMsgHandler(handler);
    is_init=true;
}
