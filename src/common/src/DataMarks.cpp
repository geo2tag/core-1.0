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
/*! ---------------------------------------------------------------
 *
 *
 * \file DataMarks.cpp
 * \brief DataMarks implementation
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include <math.h>
//#include <cstring>

#include "DataMarks.h"

void Tag::setDescription(const QString& s)
{
  m_description = s;
}


const QString& Tag::getDescription() const
{
  return m_description;
}


Tag::Tag(double altitude, double latitude, double longitude, QString label,
QString description, QString url, QDateTime time):
m_altitude(altitude),
m_latitude(latitude),
m_longitude(longitude),
m_label(label),
m_description(description),
m_url(url),
m_time(time)
{

}

Tag::Tag(const Tag &tag)
{
    copyFrom(tag);
}

Tag &Tag::operator=(const Tag &obj)
{
    copyFrom(obj);
    return *this;
}


//qlonglong Tag::getId() const
//{
//  // Database doesn't contain 0 in sequences, see scripts/base.sql
//  return 0;
//}


void Tag::setUser(const common::BasicUser &user)
{
  m_user=user;
}


//void Tag::setSession(const Session& session)
//{
//  m_session=session;
//}


void Tag::setChannel(const Channel& channel)
{
    m_channel=channel;
}

void Tag::copyFrom(const Tag &obj)
{
    m_altitude = obj.m_altitude;
    m_latitude = obj.m_latitude;
    m_longitude = obj.m_longitude;
    m_label = obj.m_label;
    m_description = obj.m_description;
    m_url = obj.m_url;
    m_time = obj.m_time;
    m_user = obj.m_user;
    m_channel = obj.m_channel;
}


Tag::~Tag()
{

}


double Tag::getLatitude() const
{
  return m_latitude;
}


void Tag::setLatitude(const double& lat)
{
  m_latitude = lat;
}


double Tag::getLongitude() const
{
  return m_longitude;
}


void Tag::setLongitude(const double &lon)
{
  m_longitude = lon;
}


double Tag::getAltitude() const
{
  // TODO
  return m_altitude;
}


void Tag::setAltitude(const double& alt)
{
  m_altitude = alt;
}


const QString& Tag::getLabel() const
{
  return m_label;
}


void Tag::setLabel(const QString& label)
{
  m_label = label;
}


const QString& Tag::getUrl() const
{
  return m_url;
}


void Tag::setUrl(const QString& url)
{
  m_url = url;
}


const QDateTime& Tag::getTime() const
{
  return m_time;
}


void Tag::setTime(const QDateTime& time)
{
  m_time = time;
}


common::BasicUser Tag::getUser() const
{
  return m_user;
}

//Session Tag::getSession() const
//{
//  return m_session;
//}


Channel Tag::getChannel() const
{
  return m_channel;
}


double deg2rad(double deg)
{
  return (deg * M_PI / 180);
}


double rad2deg(double rad)
{
  return (rad * 180 / M_PI);
}


double Tag::getDistance(double lat1, double lon1, double lat2, double lon2)
{
  double theta, dist;
  theta = lon1 - lon2;
  dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
  dist = acos(dist);
  dist = rad2deg(dist);
  dist = dist * 60 * 1.1515;            // in miles
  dist = dist * 1.609344;               // in kilometers
  return (dist);
}


bool operator<(const QSharedPointer<Tag> &a, const QSharedPointer<Tag> &b)
{
  return a->getTime() < b->getTime();
}


/* ===[ End of file  ]=== */
