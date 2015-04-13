#ifndef SERVICEFILEPARSEREXCEPTION_H
#define SERVICEFILEPARSEREXCEPTION_H

#include "BasicException.h"




class ServiceFileParserException: public BasicException
{
public:
    ServiceFileParserException(const QString& message):BasicException(message){}
};

#endif // SERVICEFILEPARSEREXCEPTION_H
