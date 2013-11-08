/*
 * Copyright 2010-2011  OSLL osll@osll.spb.ru
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */
/*----------------------------------------------------------------- !
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "ChangePasswordRequestJSON.h"
#include "JsonUser.h"

#include <qjson/parser.h>
#include <qjson/serializer.h>

ChangePasswordRequestJSON::ChangePasswordRequestJSON(QObject *parent) : JsonSerializer(parent)
{
}


ChangePasswordRequestJSON::ChangePasswordRequestJSON(const common::BasicUser& user, const QString& newPassword, QObject *parent)
: JsonSerializer(parent)
{
  m_users.push_back(user);
  m_newPassword = newPassword;
}


QByteArray ChangePasswordRequestJSON::getJson() const
{
  QJson::Serializer serializer;
  QVariantMap obj;

  Q_ASSERT(m_users.size()>0);

  obj.insert("login", m_users.at(0).getLogin());
  obj.insert("password", m_users.at(0).getPassword());

  obj.insert("new_password", m_newPassword);

  return serializer.serialize(obj);
}


bool ChangePasswordRequestJSON::parseJson(const QByteArray& data)
{
  clearContainers();

  QJson::Parser parser;
  bool ok;
  QVariantMap result = parser.parse(data, &ok).toMap();
  if (!ok) return false;

  QString newPassword = result["new_password"].toString();
  QString password = result["password"].toString();
  QString login = result["login"].toString();

  if (login.isEmpty() || password.isEmpty() || newPassword.isEmpty())
  {
    return false;
  }

  m_newPassword = newPassword;
  m_users.push_back(common::BasicUser(login,password));
  return true;
}

QString ChangePasswordRequestJSON::getNewPassword() const{
  return m_newPassword;
}
