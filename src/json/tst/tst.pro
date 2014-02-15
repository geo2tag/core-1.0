include(../../../config.pri)

INCLUDEPATH = ../inc/ ../../common/inc/ \
              ../../json/inc/ \

SOURCES += main.cpp \
    ../src/JsonUser.cpp \
    ../../common/src/BasicUser.cpp \
    ../../common/src/User.cpp \
    ../../common/src/DataMarks.cpp \
    ../../json/src/JsonSerializer.cpp \
    ../../json/src/JsonChannel.cpp \
    ../../json/src/DefaultResponseJSON.cpp \
    ../../json/src/AvailableChannelsResponseJSON.cpp \
    ../../json/src/QuitSessionRequestJSON.cpp \
    ../../json/src/FilterSubstringRequestJSON.cpp \
    ../../json/src/QuitSessionResponseJSON.cpp \
    ../../json/src/LoadTagsResponseJSON.cpp \
    ../../json/src/FilterDefaultResponseJSON.cpp \
    Test_AvailableChannelsResponseJSON.cpp \
    Test_QuitSessionRequestJSON.cpp \
    Test_QuitSessionResponseJSON.cpp \
    Test_FilterSubstringRequestJSON.cpp \
    Test_DoublePrecisionJSON.cpp

HEADERS += JsonUser_Test.h \
    ../inc/JsonUser.h \
    ../../common/inc/BasicUser.h \
    ../../common/inc/User.h \
    ../../common/inc/DataMarks.h \
    ../../common/inc/ErrnoTypes.h \
    ../../json/inc/JsonChannel.h \
    ../../json/inc/JsonSerializer.h \
    ../../json/inc/AvailableChannelsResponseJSON.h \
    ../../json/inc/QuitSessionRequestJSON.h \
    ../../json/inc/QuitSessionResponseJSON.h \
    ../../json/inc/LoadTagsResponseJSON.h \
    ../../json/inc/FilterDefaultResponseJSON.h \
    Test_AvailableChannelsResponseJSON.h \
    Test_QuitSessionResponseJSON.h \
    Test_QuitSessionRequestJSON.h \
    Test_FilterSubstringRequestJSON.h \
    Test_DoublePrecisionJSON.h\
    ../../json/inc/DefaultResponseJSON.h 

CONFIG   += qtestlib

TEMPLATE = app

QT += testlib

TARGET = utest.suite

LIBS    +=  -lcommon -lqjson
