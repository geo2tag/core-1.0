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
    Test_AvailableChannelsResponseJSON.cpp \
    Test_QuitSessionRequestJSON.cpp \
    Test_QuitSessionResponseJSON.cpp \
    Test_FilterSubstringRequestJSON.cpp
#    ../../json/src/RegisterUserRequestJSON.cpp \
#    ../../json/src/RegisterUserResponseJSON.cpp \
#    ../../json/src/RestorePasswordRequestJSON.cpp \
#    Test_RegisterUserResponseJSON.cpp \
#    Test_RegisterUserRequestJSON.cpp \
#    Test_RestorePasswordRequestJSON.cpp \
#    Test_RestorePasswordResponseJSON.cpp
HEADERS += JsonUser_Test.h \
    ../inc/JsonUser.h \
    ../../common/inc/BasicUser.h \
    ../../common/inc/User.h \
    ../../common/inc/DataMarks.h \
    ../../json/inc/JsonChannel.h \
    ../../json/inc/JsonSerializer.h \
    ../../json/inc/AvailableChannelsResponseJSON.h \
    ../../json/inc/QuitSessionRequestJSON.h \
    ../../json/inc/QuitSessionResponseJSON.h \
    Test_AvailableChannelsResponseJSON.h \
    Test_QuitSessionResponseJSON.h \
    Test_QuitSessionRequestJSON.h \
    Test_FilterSubstringRequestJSON.h \
    ../../json/inc/DefaultResponseJSON.h 
#    ../../json/inc/RestorePasswordRequestJSON.h \
#    ../../json/inc/RestorePasswordResponseJSON.h \
#    ../../json/inc/RegisterUserRequestJSON.h \
#    ../../json/inc/RegisterUserResponseJSON.h \
#    Test_RegisterUserResponseJSON.h \
#    Test_RegisterUserRequestJSON.h \
#    Test_RestorePasswordResponseJSON.h \
#    Test_RestorePasswordRequestJSON.h

CONFIG   += qtestlib

TEMPLATE = app

QT += testlib

TARGET = utest.suite

LIBS    +=  -lcommon -lqjson
