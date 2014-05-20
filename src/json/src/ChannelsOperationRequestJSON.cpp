#include "ChannelsOperationRequestJSON.h"
#include "servicelogger.h"
#include <qjson/parser.h>
#include <qjson/serializer.h>
#include <QRegExp>


ChannelsOperationRequestJSON::ChannelsOperationRequestJSON(QObject *parent): JsonSerializer(parent)
{
}


bool ChannelsOperationRequestJSON::parseJson(const QByteArray &data)
{
    clearContainers();

    QJson::Parser parser;
    bool ok;
    QVariantMap result = parser.parse(data, &ok).toMap();

    if(!ok) return false;

    QString auth_token = result["auth_token"].toString();
    QString formula = result["formula"].toString();
    formula.simplified();
    formula.replace(" ", "");

    QStringList tokens;
    QString token = "";
    foreach (QChar c, formula)
    {
        if (c == QChar('(') || c == QChar(')') || c == QChar('|') || c == QChar('&') || c == QChar('\\'))
        {
            if (!token.isEmpty())
            {
                tokens.append(token);
                tokens.append(QString(c));
                token = "";
            } else {
                tokens.append(QString(c));
            }
        } else {
            token.append(c);
        }
    }

    if (!token.isEmpty()) {
        tokens.append(token);
    }

    foreach (QString s, tokens) {
       if(s != "(" && s != ")" && s != "|" && s != "&" && s != "\\") {
           m_channels.push_back(Channel(s));
       }
    }

    m_token = auth_token;
    this->formula = tokens;

    return true;
}

QStringList ChannelsOperationRequestJSON::getFormula() const
{
    return formula;
}
