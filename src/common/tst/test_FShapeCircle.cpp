#include "test_FShapeCircle.h"
#include <QtTest/QTest>
#include "DataMarks.h"
#include "FShapeCircle.h"

#define POINT_IN_REGION Tag(0.,1.,1.)
#define POINT_OUT_REGION Tag(0.,1000.,1000.)

#define TEST_CENTER_LAT 0
#define TEST_CENTER_LON 0
#define TEST_RADIUS 1000

namespace Test
{


      void Test_FShapeCircle::testFiltration()
      {

        // Object for testing
        FShapeCircle *  m_tstObject = new FShapeCircle(TEST_CENTER_LAT, TEST_CENTER_LON, TEST_RADIUS);

        QVERIFY(m_tstObject->filtrate(POINT_IN_REGION));
        QVERIFY(!m_tstObject->filtrate(POINT_OUT_REGION));
      }


}


//QTEST_APPLESS_MAIN(Test::FShapeCircleTest)
//#include "test_FShapeCircle.moc"
