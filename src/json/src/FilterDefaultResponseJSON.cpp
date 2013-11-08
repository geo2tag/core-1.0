/*
 * Copyright 2011  bac1ca  bac1ca89@gmail.com
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
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

/*! ---------------------------------------------------------------
 *
 * \file FilterDefaultResponseJSON.cpp
 * \brief FilterDefaultResponseJSON implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "FilterDefaultResponseJSON.h"

#include <QVariant>
#include <QDebug>
#include "servicelogger.h"

#if !defined(Q_OS_SYMBIAN) && !defined(Q_WS_SIMULATOR)
#include <qjson/parser.h>
#include <qjson/serializer.h>
#else
#include "parser.h"
#include "serializer.h"
#endif

#include "User.h"
#include "Channel.h"

#include "JsonUser.h"
#include "JsonChannel.h"
#include "JsonDataMark.h"


FilterDefaultResponseJSON::FilterDefaultResponseJSON(QObject *parent) : JsonSerializer(parent)
{
}


FilterDefaultResponseJSON::~FilterDefaultResponseJSON()
{
}


QList<Tag> FilterDefaultResponseJSON::getTags() const
{
  return m_tags;
}


void FilterDefaultResponseJSON::setTags(const QList<Tag>& tags)
{
  Tag t;
  foreach(t, tags)
  {
	m_tagMap.insert(t.getChannel(),t);
  }
}


bool FilterDefaultResponseJSON::parseJson(const QByteArray& data)
{
  clearContainers();

  QJson::Parser parser;
  bool ok;
  QVariantMap result = parser.parse(data, &ok).toMap();

  if (!ok) return false;

  result["errno"].toInt(&ok);
  if (!ok) return false;
  m_errno = result["errno"].toInt(&ok);

  QVariantList channelsList = result["channels"].toList();
  int size = channelsList.size();

  for (int i = 0; i < size; i++)
  {
    QVariantMap channelDesc = channelsList.at(i).toMap();
    QVariantMap channelInternal = channelDesc["channel"].toMap();
    QVariantList markList = channelInternal["items"].toList();
    QString channelName = channelInternal["name"].toString();

    Channel channel(channelName,"dummy channel[LoadTagsResponse]");

    for(int j=0; j<markList.size(); j++)
    {
      QVariantMap markMap = markList.at(j).toMap();

      QString title = markMap["title"].toString();
      QString link = markMap["link"].toString();
      QString description = markMap["description"].toString();
      double altitude = markMap["altitude"].toString().toDouble(&ok);
      if (!ok) return false;
      double latitude = markMap["latitude"].toString().toDouble(&ok);
      if (!ok) return false;
      double longitude = markMap["longitude"].toString().toDouble(&ok);
      if (!ok) return false;

      QString userName = markMap["user"].toString();
      QString timeStr =  markMap["pubDate"].toString();
      QDateTime time = QDateTime::fromString(timeStr, "dd MM yyyy HH:mm:ss.zzz");
      //time.setTimeSpec(Qt::UTC);
   
      Tag tag = Tag(altitude,  latitude, longitude, title, description, link, time);
      m_tagMap.insert(channel,tag);
      //tag.setChannel(channel);      
      //m_tags << tag;
    }
  }
  return true;
}


QByteArray FilterDefaultResponseJSON::getJson() const
{
  QJson::Serializer serializer;
  QVariantMap obj;

//  QList<QSharedPointer<Channel> > hashKeys = m_hashMap.uniqueKeys();
  QVariantList jchannels;
  QList<Channel> channels = m_tagMap.uniqueKeys();
  DEBUG() << "Sizeof channels  " << channels.size();

  for(int i=0; i<channels.size(); i++)
  {
    QList<Tag> tags = m_tagMap.values(channels.at(i));
    qSort(tags);
    QVariantList jtags;
    QVariantMap jchannel;
    QVariantMap channel;

    for(int j=0; j<tags.size(); j++)
    {
      Tag tag = tags.at(j);
      QVariantMap jtag;
      jtag["title"] = tag.getLabel();
      jtag["link"] = tag.getUrl();
      jtag["description"] = tag.getDescription();
      jtag["latitude"] = tag.getLatitude();
      jtag["longitude"] = tag.getLongitude();
      jtag["altitude"] = tag.getAltitude();
      jtag["user"] = tag.getUser().getLogin();
      jtag["pubDate"] = tag.getTime().toString("dd MM yyyy HH:mm:ss.zzz");
      jtags.append(jtag);
    }
    channel["items"] = jtags;
    channel["name"] = channels[i].getName();
    jchannel["channel"] = channel;
    jchannels.append(jchannel);
  }
  obj["channels"] = jchannels;
  obj.insert("errno", getErrno());
  return serializer.serialize(obj);
}

