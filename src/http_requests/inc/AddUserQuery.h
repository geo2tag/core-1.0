#ifndef ADDUSERQUERY_H
#define ADDUSERQUERY_H
#include <QObject>
#include <QString>
#include "DefaultQuery.h"
#include "Session.h"

class AddUserQuery: public DefaultQuery
{
    Q_OBJECT

    QString m_login;
    QString m_password;
    QString m_email;

    //!< full information about user
    common::BasicUser m_user;
    Session m_session;

    virtual QString getUrl() const;
    virtual QByteArray getRequestBody() const;

private Q_SLOTS:

    virtual void processResponse(const QByteArray &data);

public:

    AddUserQuery(const QString& login, const QString& password, QObject *parent = 0, const QString& email = "unknown");

    explicit AddUserQuery(QObject *parent = 0);

    void setQuery(const QString& login, const QString& password);
    void setQuery(const QString& login, const QString& password, const QString& email);

    common::BasicUser getUser() const;
    Session getSession() const;

    ~AddUserQuery();

Q_SIGNALS:

};
#endif                                  // ADDUSERQUERY_H
