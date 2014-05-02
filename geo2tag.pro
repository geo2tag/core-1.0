TEMPLATE = subdirs

include(config.pri)

libriak.makefile = Makefile.libriak
libriak.subdir = 3rdparty/libriak/

SUBDIRS += libriak \
	 src 

CONFIG += mobility ordered
MOBILITY = location

QMAKE_CFLAGS_DEBUG += "-g3 -Werror"
QMAKE_CXXFLAGS_DEBUG +="-g3 -Werror"

