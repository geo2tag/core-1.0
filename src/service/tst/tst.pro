include(../../../config.pri)

DEPENDDEPATH = . \
              ../inc/ \ 
              ../src/ \
              ../../common/inc/ \
              ../../common/src/ \
              ../../json/inc \
              ../../json/src 

INCLUDEPATH = . ../inc/ ../../common/inc/ \
              ../../json/inc

SOURCES +=   ../src/Geo2tagDatabase.cpp \
            ../src/QueryExecutor.cpp \
           ../src/UpdateThread.cpp \
           ../src/UserInternal.cpp \
           ../src/DataMarkInternal.cpp \
           ../src/SessionInternal.cpp \
           ../src/ChannelInternal.cpp \
           main.cpp \
    Test_GetGuidDbObjectsCollection.cpp \
    ../src/DbObjectsCollection.cpp \
    ../src/MetaCache.cpp \
    ../src/server.cpp \
    ../src/DynamicCastFailure.cpp \
    ../processors/ProcessorsFilters.cpp \
    ../processors/ProcessorsCore.cpp \
    Test_RiakInteractor.cpp \
    ../src/RiakInteractor.cpp \

#          Test_QueryExecutor.cpp

HEADERS += ../inc/QueryExecutor.h \
           ../inc/Geo2tagDatabase.h \
           ../inc/UpdateThread.h \
           ../inc/UserInternal.h \
           ../inc/DataMarkInternal.h \
           ../inc/SessionInternal.h \
           ../inc/ChannelInternal.h \
#            UpdateThread_Test.h \
    Test_GetGuidDbObjectsCollection.h \
    ../inc/DbObjectsCollection.h \
    ../inc/MetaCache.h \
    ../inc/server.h \
    ../inc/DynamicCastFailure.h \
    Test_RiakInteractor.h \
    ../inc/RiakInteractor.h \

#            Test_QueryExecutor.h \

CONFIG   += qtestlib

TEMPLATE = app

QT += testlib
QT += sql

INCLUDEPATH += "/home/rodion/riak-1.4.2/client_lib/riak-cxx-client-master/riak_client"
INCLUDEPATH += "/home/rodion/riak-1.4.2/client_lib/riak-cxx-client-master"


## GT_779 This is not correct test format for pulse. refactoring is required
TARGET = test.DbInteraction



LIBS -= -L/usr/lib
LIBS += -lcommon -lwikigpsJson -lfcgi -lriak_client
message($$LIBS)

RESOURCES += \
    TestBlobs.qrc
