#include "Test_GetGuidDbObjectsCollection.h"
#include <QTest>


void Test::Test_GetGuidDbObjectsCollection::getGuid()
{
    QString uuid = common::DbObjectsCollection::getGuid();
    qDebug() << uuid;
    QCOMPARE(uuid.length(), 36);
}
