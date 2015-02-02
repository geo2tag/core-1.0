TEMPLATE = lib
include(../../config.pri)

TARGET = ../lib/videoManager
DEPENDPATH += . inc src

target.path = /usr/lib
INSTALLS += target
INCLUDEPATH += . ./inc 

# Input
HEADERS += \
           inc/Terminal.h \
           inc/VideoFile.h \
           inc/VideoManager.h \

SOURCES += \
           src/Terminal.cpp \
           src/VideoFile.cpp \
           src/VideoManager.cpp \
           


linux: {
    DEFINES += DESKTOP_STYLE
}

