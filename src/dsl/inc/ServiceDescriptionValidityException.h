#ifndef SERVICEDESCRIPTIONVALIDITYEXCEPTION_H
#define SERVICEDESCRIPTIONVALIDITYEXCEPTION_H

#include "BasicException.h"

class ServiceDescriptionValidityException: public BasicException
{
public:
    ServiceDescriptionValidityException(const QString& message):BasicException(message){}

};

#endif // SERVICEDESCRIPTIONVALIDITYEXCEPTION_H
