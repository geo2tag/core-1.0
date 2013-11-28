#ifndef STRINGENUM_H
#define STRINGENUM_H

#include <QDebug>
#include "ServiceFileParserException.h"

template <typename E, int ELEMENTS_NUMBER>
        class StringEnum
{
public:
    ////////// HACK ////////////////
    static char** STRING_NAMES;

    static char* ERROR_MESSAGE;
    ///////////////////////////////


    E value;

    StringEnum(){
    }

    StringEnum(const QString& string){
        value = fromString(string);
    }

    E fromString(const QString& string){
        for (int i = 0; i< ELEMENTS_NUMBER; i++){
	qDebug() << string << STRING_NAMES[i];
            if (QString(STRING_NAMES[i]).compare( string, Qt::CaseInsensitive) == 0)
                return (E)i;
        }

        throw ServiceFileParserException(ERROR_MESSAGE);
    }

    const char * toString(){
        return STRING_NAMES[value];
    }

};

template<typename E, int ELEMENTS_NUMBER> char** StringEnum<E, ELEMENTS_NUMBER>::STRING_NAMES;
template<typename E, int ELEMENTS_NUMBER> char* StringEnum<E, ELEMENTS_NUMBER>::ERROR_MESSAGE;


#endif // STRINGENUM_H
