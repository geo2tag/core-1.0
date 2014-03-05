TEMPLATE = subdirs

include(config.pri)

libriak.subdir = 3rdparty/libriak
libriak.makefile = Makefile.libriak

SUBDIRS += libriak
SUBDIRS += src

CONFIG += mobility ordered
MOBILITY = location

QMAKE_CFLAGS_DEBUG += "-g3 -Werror"
QMAKE_CXXFLAGS_DEBUG +="-g3 -Werror"

