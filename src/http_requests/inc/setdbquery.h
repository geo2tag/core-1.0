#ifndef SETDBQUERY_H
#define SETDBQUERY_H
#include <QString>
#include <QObject>
#include "Session.h"
#include "DefaultQuery.h"

class SetDBQuery : public DefaultQuery
{
    Q_OBJECT

    QString m_dbName;
    Session m_session;

    virtual QString getUrl() const;
    virtual QByteArray getRequestBody() const;

public:

    SetDBQuery(const Session& session, const QString& dbName, QObject *parent = 0);
    explicit SetDBQuery(QObject *parent = 0);

    void setDBName(const QString &dbName);
    QString getDBName()const;

    void setSession(const Session &session);
    Session getSession() const;
};

#endif // SETDBQUERY_H
