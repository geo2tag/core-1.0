#include "test_FShapeRectangle.h"
#include <QtTest/QTest>

#include "Region.h"
#include "DataMarks.h"
#include "FShapeRectangle.h"

#define POINT_IN_REGION Tag(0.,500.,500.)
#define POINT_OUT_REGION Tag(0.,1500.,500.)

#define TEST_LAT 0
#define TEST_LON 0
#define TEST_WIDTH 1000
#define TEST_HEIGHT 1000


namespace Test
{


      void Test_FShapeRectangle::testFiltration()
      {
        // Object for testing
        FShapeRectangle *  m_tstObject = new FShapeRectangle(TEST_LAT, TEST_LON, TEST_WIDTH, TEST_HEIGHT);


        QVERIFY(m_tstObject->filtrate(POINT_IN_REGION));
        QVERIFY(!m_tstObject->filtrate(POINT_OUT_REGION));

	delete m_tstObject;
      }


}


//QTEST_APPLESS_MAIN(Test::FShapeRectangleTest)
//#include "test_FShapeRectangle.moc"
