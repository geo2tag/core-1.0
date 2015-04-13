/*
 * Copyright 2010-2013  OSLL osll@osll.spb.ru
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
/*!
 * \file Test_DoublePrecisionJSON.cpp
 * \brief Test suite for #GT-1046 class
 *
 * PROJ: OSLL/geo2tag
 * ----------------------------------------------------------- */

#include "Test_DoublePrecisionJSON.h"
#include "ErrnoTypes.h"

#define TEST_NUMBER 012.0123456789
#define TEST_TAG Tag(TEST_NUMBER,TEST_NUMBER,TEST_NUMBER)

namespace Test
{
	
	Test_DoublePrecisionJSON::Test_DoublePrecisionJSON():QObject(){
		TEST_LIST.append(TEST_TAG);
	}

	void Test_DoublePrecisionJSON::testLoadTagsResponse(){
		LoadTagsResponseJSON response;
		response.setData(TEST_LIST);
		response.setErrno(SUCCESS);
		
		qDebug() << response.getJson();
		QByteArray json  = response.getJson();
		response.setData(QList<Tag>());
		response.parseJson(json);
		
		Tag resultTag = response.getData()[0];
		QCOMPARE(TEST_TAG.getLatitude(), resultTag.getLatitude());
		QCOMPARE(TEST_TAG.getLongitude(), resultTag.getLongitude());
		QCOMPARE(TEST_TAG.getAltitude(), resultTag.getAltitude());
	}

	void Test_DoublePrecisionJSON::testFilterDefaultResponse(){

		FilterDefaultResponseJSON response;
		response.setTags(TEST_LIST);
		
		qDebug() << response.getJson();

		QByteArray json  = response.getJson();
		response.setTags(QList<Tag>());
		response.parseJson(json);
		
		Tag resultTag = response.getTags()[0];
		QCOMPARE(TEST_TAG.getLatitude(), resultTag.getLatitude());
		QCOMPARE(TEST_TAG.getLongitude(), resultTag.getLongitude());
		QCOMPARE(TEST_TAG.getAltitude(), resultTag.getAltitude());
	}
}                                       // end of namespace Test
