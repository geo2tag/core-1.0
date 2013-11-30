#ifndef SERVICEFILEPARSER_H
#define SERVICEFILEPARSER_H

#include "ServiceDescription.h"
#include <QString>

#define COMMENT_SYMBOL "#"

class ServiceFileParser
{   
    ServiceDescription m_serviceDescription;
    int getDividerPosition(const QString& line);

    QString removeComments(const QString& line);


    QString extractPrefix(const QString& line);
    QString extractMainPart(const QString& line);
    void processLine(const QString& line);
public:

     ServiceDescription parse( const QString& file);

};

#endif
