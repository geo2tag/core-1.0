#ifndef SERVICELOGGER_H
#define SERVICELOGGER_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QSharedPointer>

//Class, which writes messages to log id their writing is enabled in config
class Log : public QObject
{

    explicit Log();
public:
    /*All logging facilities and severites. To add new facility, add it to enum,
     *and assign it a name in Log::init() in servicelogger.cpp
     *IMPORTANT: Do not assign numbers to enum's values
     *undefined facility stands for log captured from qWarning(), qDebug(), qCritical(), qFatal()
     */
    enum Facility
    {
      undefined_fac,
      test_fac,
      test2_fac,
      facility_last_element__
    };
    enum Severity
    {
      debug_sev,
      warning_sev,
      critical_sev,
      fatal_sev,
      severity_last_element__
    };
    static void init();
};
//Class, used in logging macros to assemble a string from  macros' call and pass it to Log
class MessageAssembler
{
public:
    Log::Severity msg_sev;
    Log::Facility msg_fac;
    QString contents;//string which will be passed to Log
    QDebug stream;
    MessageAssembler(Log::Severity _msg_sev,Log::Facility _msg_fac);
    ~MessageAssembler();
    //Returns a temporary instance of assembler
    static QSharedPointer<MessageAssembler> get_instance(Log::Severity _msg_sev,Log::Facility _msg_fac);
    //Passes contents of reult to Log
    void operator+=(MessageAssembler& result);
};
//TODO: Find a way to put this definition to servicelogger.cpp.
template<typename msg_type> MessageAssembler& operator<<(MessageAssembler& target,const msg_type msg)
{
    target.stream<<msg<<" ";
    return target;
}
namespace NLog
{
    extern bool is_init;
    //Is log with a certain combination of facility/severity enabled
    extern bool log_enabled[Log::facility_last_element__][Log::severity_last_element__];
    //Name of facilities and severeties of log, printed in log file. Also, these names without spaces are used in config file
    extern QString severity_name[Log::severity_last_element__];
    extern QString facility_name[Log::facility_last_element__];

    extern MessageAssembler assembler;//Can be replaced with MessageAssembler::get_instance(...,...)
}

//TODO:  add facilities to calls of logging macros and delete __ after names of the following macros. Delete logging macros with no arguments
#define DEBUG__(facility) NLog::assembler+=(*MessageAssembler::get_instance(Log::debug_sev,facility))<< __FILE__<<":"<<__LINE__
#define WARNING__(facility) NLog::assembler+=(*MessageAssembler::get_instance(Log::warning_sev,facility))<< __FILE__<<":"<<__LINE__
#define CRITICAL__(facility) NLog::assembler+=(*MessageAssembler::get_instance(Log::critical_sev,facility))<< __FILE__<<":"<<__LINE__
//Note: FATAL() macro will terminate the application
#define FATAL__(facility) NLog::assembler+=(*MessageAssembler::get_instance(Log::fatal_sev,facility))<< __FILE__<<":"<<__LINE__


#define DEBUG() DEBUG__(Log::test_fac)
#define WARNING() WARNING__(Log::test_fac)
#define CRITICAL() CRITICAL__(Log::test_fac)
#define FATAL() FATAL__(Log::test_fac)
#endif // SERVICELOGGER_H
