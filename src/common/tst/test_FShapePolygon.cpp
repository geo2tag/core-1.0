#include "test_FShapePolygon.h"
#include <QtTest/QTest>

#include "Region.h"
#include "DataMarks.h"
#include "FShapePolygon.h"

#define POINT_IN_REGION Tag(0.,500.,500.)
#define POINT_OUT_REGION Tag(0.,1500.,1500.)

#define TAG1 Tag(0.,0.,0.)
#define TAG2 Tag(0.,1000.,0.)
#define TAG3 Tag(0.,1200.,200.)
#define TAG4 Tag(0.,200.,1200.)
#define TAG5 Tag(0.,0.,1000.)

namespace Test
{


      void Test_FShapePolygon::testFiltration()
      {
        // Object for testing
        FShapePolygon *  m_tstObject = new FShapePolygon();


        m_tstObject->addPoint(1, TAG1.getLatitude(), TAG1.getLongitude());
        m_tstObject->addPoint(2, TAG2.getLatitude(), TAG2.getLongitude());
        m_tstObject->addPoint(3, TAG3.getLatitude(), TAG3.getLongitude());
        m_tstObject->addPoint(4, TAG4.getLatitude(), TAG4.getLongitude());
        m_tstObject->addPoint(5, TAG5.getLatitude(), TAG5.getLongitude());

        QVERIFY(m_tstObject->filtrate(POINT_IN_REGION));
        QVERIFY(!m_tstObject->filtrate(POINT_OUT_REGION));

	delete m_tstObject;
      }


}


//QTEST_APPLESS_MAIN(Test::FShapePolygonTest)
//#include "test_FShapePolygon.moc"
