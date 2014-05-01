TEMPLATE = subdirs
include(../config.pri)

libriak.makefile = Makefile.libriak
libriak.subdir = libriak/

CONFIG += ordered
message($$LIBS)

SUBDIRS += \
						 common \	
						 libriak\
             json \
             http_requests \
             service \
      	     common/tst\
             json/tst\ 
#            service/tst
#	     maps \
#            http_requests/tst_http_requests \
#            tracker/clients/meego/tst \
