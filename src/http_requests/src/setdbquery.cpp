#include "setdbquery.h"
#include "defines.h"
#include "SetDbRequestJSON.h"

SetDBQuery::SetDBQuery(const Session &session, const QString &dbName, QObject *parent)
    :DefaultQuery(parent),
      m_dbName(dbName),
      m_session(session)
{
}

SetDBQuery::SetDBQuery(QObject *parent): DefaultQuery(parent)
{
}


QString SetDBQuery::getUrl() const
{
    return getServerUrl() + QString("service/setdb");
}

QByteArray SetDBQuery::getRequestBody() const
{
    SetDbRequestJSON request;
    request.setDbName(m_dbName);
    request.setSessionToken(m_session.getSessionToken());
    return request.getJson();
}


void SetDBQuery::setDBName(const QString &dbName)
{
    m_dbName = dbName;
}

QString SetDBQuery::getDBName() const
{
    return m_dbName;
}

void SetDBQuery::setSession(const Session &session)
{
    m_session = session;
}

Session SetDBQuery::getSession() const
{
    return m_session;
}
