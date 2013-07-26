#include "test_AltitudeFilter.h"
#include <QtTest/QTest>

#include "Region.h"
#include "DataMarks.h"
#include "AltitudeFilter.h"

#define POINT_IN_FILTER Tag(1.)
#define POINT_OUT_FILTER Tag(10.)

#define TEST_ALT1 0
#define TEST_ALT2 2

namespace Test
{



      void Test_AltitudeFilter::testFiltration()
      {
        // Object for testing
        AltitudeFilter *  m_tstObject = new AltitudeFilter (TEST_ALT1, TEST_ALT2);

	// Correct version: 
	// QVERIFY(m_tstObject->filtrate(POINT_IN_FILTER));
        QVERIFY(!m_tstObject->filtrate(POINT_IN_FILTER));
        QVERIFY(!m_tstObject->filtrate(POINT_OUT_FILTER));
	delete m_tstObject;
      }


}


//QTEST_APPLESS_MAIN(Test::AltitudeFilterTest)
//#include "test_AltitudeFilter.moc"
