#include "test_TagType.h"
#include "DataMarks.h"

namespace Test
{

void Test_TagType::getTagType()
{
    Tag tag;
    QCOMPARE(tag.checkBinaryTag(), TAG_COMMON);
    tag.setLabel("#BLOB");
    QCOMPARE(tag.checkBinaryTag(), TAG_BINARY);
    tag.setLabel("testLabel, secondTestLabel, #BLOB: idblob123, nameBlob");
    QCOMPARE(tag.checkBinaryTag(), TAG_BINARY);
}

}
