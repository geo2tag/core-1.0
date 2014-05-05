#include "ServiceFileParser.h"
#include "ServiceFileParserException.h"
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QDebug>
#include <QtGlobal>

ServiceDescription ServiceFileParser::parse( const QString& file){

    QFile inputFile(file);
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        while ( !in.atEnd() )
        {
            QString line = in.readLine();
            qDebug() << line;
            processLine(line);
        }
    }else{
        qFatal(QString("Unable to open %1").arg(file).toStdString().c_str());
    }

    inputFile.close();

    m_serviceDescription.checkDescriptionValidity();

    return m_serviceDescription;

}

int ServiceFileParser::getDividerPosition(const QString& line){
   int index = line.indexOf(QRegExp("[(=]"));
   if (index == -1)
       throw ServiceFileParserException("Error parsing line");

   return index;
}

QString ServiceFileParser::extractPrefix(const QString& line){
    int index = getDividerPosition(line);
    QString result = line.left(index);

    return result;
}

QString ServiceFileParser::extractMainPart(const QString& line){
    int index = getDividerPosition(line);  

    QChar divider = line.at(index);
    int lastIndex = -1;
    if (divider == '(')
        lastIndex = line.length() - index - 2;

    QString result = line.mid(index+1, lastIndex);

    return result;
}


QString ServiceFileParser::removeComments(const QString& line){
    QString result = line;
    int index = line.indexOf(COMMENT_SYMBOL);
    if (index > -1){
        result = ( (index == 0) ? QString("") :  line.left(index -1) );
    }
    qDebug() << "After comment removing: " << result;
    qDebug() << "Size = " << result.trimmed().size();
    return result;
}

void ServiceFileParser::processLine(const QString& line){
    QString lineToProcess = removeComments(line);
    // Skip empty lines
    if (lineToProcess.trimmed().size() == 0)
        return;

    try{
        QString prefix = extractPrefix(lineToProcess);
        QString mainPart = extractMainPart(lineToProcess);
        m_serviceDescription.addDescriptionEntry(prefix.trimmed(), mainPart.trimmed());
    } catch (ServiceFileParserException& e){
        qDebug() << "At line:\n" << line;
        qFatal( e.errorMessage.toStdString().c_str());

    }


}
