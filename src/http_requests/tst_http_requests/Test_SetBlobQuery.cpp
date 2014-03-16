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

#include "Test_SetBlobQuery.h"
#include "signals.h"
#include "Session.h"
#include "User.h"
#include "SettingsStorage.h"

#include <QString>
#include <QTimer>
#include <QDateTime>

namespace Test
{
void Test_SetBlobQuery::response()
{
    SettingsStorage::init();
    SettingsStorage::setValue("server_url", QVariant("http://127.0.0.1/"), "common");
    SetBlobQuery query(this);
    common::User user("Paul", "test");
    Session session("5136257300aadc645a6aa24295bc60b3", QDateTime::currentDateTime().toUTC(), user);
    query.setSession(session);
    query.setGuid("96e66ef5a7974ded9bcec23d25930574");
    query.setBlob("Blob from Test_SetBlobQuery.cpp");
    query.doRequest();
    //connect(&query, SIGNAL(errorOccured(QString)), this, SLOT(ok()));
    QVERIFY(waitForSignal(&query, SIGNAL(errorOccured(int)), 5000));
    qDebug() << query.getErrno();
}

}                                       // end of namespace Test

