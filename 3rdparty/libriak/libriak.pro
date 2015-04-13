MAKEFILE = Makefile.libriak

target.path = /usr/lib
target.files = ./lib/libriak_client.so ./lib/libriak_client.so.1 ./lib/libriak_client.so.1.0.0
INSTALLS += target

ATPROJ_SRCDIR = ./

autoreconf.target = $${ATPROJ_SRCDIR}/configure
autoreconf.commands = cd $${ATPROJ_SRCDIR} && autoreconf

aclocal.target = $${ATPROJ_SRCDIR}/aclocal.m4
aclocal.depends = autoreconf

automake.target = $${ATPROJ_SRCDIR}/Makefile.in
automake.depends = autoreconf

autoheader.target = $${ATPROJ_SRCDIR}/Makefile.in
autoheader.depends = autoreconf

libtoolize.target = $${ATPROJ_SRCDIR}/ltmain.sh
libtoolize.depends = autoreconf

Makefile.target = Makefile
Makefile.commands = $${ATPROJ_SRCDIR}/configure
Makefile.depends = autoreconf aclocal automake autoheader libtoolize

all.commands = make && cp ./riakc ./.libs && mkdir -p ./lib && cp ./.libs/libriak_client.so ./lib && cp ./.libs/libriak_client.so.1 ./lib && cp ./.libs/libriak_client.so.1.0.0 ./lib && pwd && ls -la ./lib
all.depends = Makefile
all.CONFIG = phony

TARGET = \\\

QMAKE_DISTCLEAN += Makefile
QMAKE_EXTRA_TARGETS += autoreconf aclocal automake autoheader libtoolize Makefile all
