#include "test_TimeFilter.h"
#include <QtTest/QTest>

#include "Region.h"
#include "DataMarks.h"
#include "TimeFilter.h"
#include <QDateTime>

#define POINT_IN_FILTER Tag(0., 0., 0., "", "", "", QDateTime::fromString("20 01 2013 00:00:00.000", "dd MM yyyy HH:mm:ss.zzz"))
#define POINT_OUT_FILTER Tag(0., 0., 0., "", "", "", QDateTime::fromString("22 01 2013 00:00:00.000", "dd MM yyyy HH:mm:ss.zzz"))

#define TEST_TIME1 QDateTime::fromString("19 01 2013 00:00:00.000",  "dd MM yyyy HH:mm:ss.zzz")
#define TEST_TIME2 QDateTime::fromString("21 01 2013 00:00:00.000",  "dd MM yyyy HH:mm:ss.zzz")

namespace Test
{


      void Test_TimeFilter::testFiltration()
      {
        // Object for testing
        TimeFilter *  m_tstObject = new TimeFilter(TEST_TIME1, TEST_TIME2);


        QVERIFY(m_tstObject->filtrate(POINT_IN_FILTER));
        QVERIFY(!m_tstObject->filtrate(POINT_OUT_FILTER));
      }


}


//QTEST_APPLESS_MAIN(Test::TimeFilterTest)
//#include "test_TimeFilter.moc"
