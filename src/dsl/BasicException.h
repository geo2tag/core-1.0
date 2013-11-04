#ifndef BASICEXCEPTION_H
#define BASICEXCEPTION_H

#include <QString>

class BasicException{
public:
    QString errorMessage;
    BasicException(const QString& message){
        errorMessage = message;
    }
};

#endif // BASICEXCEPTION_H
