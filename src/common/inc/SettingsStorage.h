/*
 * Copyright 2010-2012  OSLL osll@osll.spb.ru
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
/*! --------------------------------------------------------------------------
 * \file   SettingsStorage.h
 * \brief  Declaration of SettingStorage Class
 *         Class SettingStorage provides working with platform setting storage
 *
 * PROJ: OSLL/geo2tag
 * --------------------------------------------------------------------------- */

#ifndef SETTINGSSTORAGE_H
#define SETTINGSSTORAGE_H

#include <QString>
#include <QVariant>

class SettingsStorage
{
  private:
    QString                 m_filename;
    static SettingsStorage *s_settings;

    SettingsStorage(const QString filename);

  public:

    static void init();
    static void setValue(const QString &key, const QVariant &value, const QString &group = "");
    static QVariant getValue(const QString &key, const QVariant &defaultValue = QVariant());

  protected:
    void setFileName(const QString& filename);
    QString getFileName() const;

    virtual ~SettingsStorage();
};
#endif                                  // SETTINGSSTORAGE_H
