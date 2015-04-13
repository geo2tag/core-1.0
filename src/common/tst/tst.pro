include(../../../config.pri)

INCLUDEPATH +=../inc/ ../../json/inc/


SOURCES += \
    main.cpp \
    test_AltitudeFilter.cpp \
    test_TimeFilter.cpp \
    test_FShapeCircle.cpp \
    test_FShapeRectangle.cpp \
    test_FShapePolygon.cpp \
    ../src/Region.cpp \
    ../src/DataMarks.cpp \
    ../src/Channel.cpp \
    ../src/BasicUser.cpp \
    ../src/AltitudeFilter.cpp \
    ../src/TimeFilter.cpp \
    ../src/FShapeCircle.cpp \
    ../src/FShapePolygon.cpp \
    ../src/FShapeRectangle.cpp \ 
    test_TagType.cpp


CONFIG +=qtestlib console
CONFIG -=app_bundle

TARGET = utest.CommonTest

HEADERS += \
    test_AltitudeFilter.h \
    test_TimeFilter.h \
    test_FShapeCircle.h \
    test_FShapeRectangle.h \
    test_FShapePolygon.h \
    ../inc/Region.h \
    ../inc/GpsInfo.h \
    ../inc/DataMarks.h \
    ../inc/Channel.h \
    ../inc/BasicUser.h \
    ../inc/AltitudeFilter.h \
    ../inc/TimeFilter.h \
    ../inc/FShapeCircle.h \
    ../inc/FShapePolygon.h \
    ../inc/FShape.h \
    ../inc/FShapeRectangle.h \
    test_TagType.h

