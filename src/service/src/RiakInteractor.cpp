/*
 * Copyright 2010  OSLL osll@osll.spb.ru
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

#include "RiakInteractor.h"
#include "riak_client/cxx/riak_client.hpp"
#include "riak_object.h"
#include <QFile>
#include <QDataStream>


RiakInteractor::RiakInteractor(const std::string &host, const std::string &port)
{
    m_c = riak::new_client(host, port);
}

std::string RiakInteractor::getData(const std::string bucketName, const std::string key, const std::string fileName, int r)
{
    riak::result_ptr fr(m_c->fetch(bucketName, key, r));
    std::string dataString;
    dataString = fr->contents()[0].value();
    QFile file(fileName.c_str());
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out.writeRawData(dataString.c_str(), dataString.length());
    return dataString;
}

void RiakInteractor::putData(const std::string bucketName, const std::string key, const std::string data, const std::string & contentType, bool returnBody)
{
    riak::store_params sp;
    sp.w(3).dw(3).return_body(returnBody);
    riak::object_ptr o;
    o = riak::make_object(bucketName, key, data);
    riak::riak_metadata md;
    md.content_type(contentType);
    o->update_content().metadata(md);
    m_c->store(o, sp);
}

bool RiakInteractor::createBucket(const std::string & bucketName, int n_val, bool allow_mult)
{
    bool ping;
    riak::bucket_properties properties;
    properties.allow_mult(allow_mult);
    properties.n_val(n_val);
    ping = m_c->set_bucket(bucketName, properties);
    return ping;
}
