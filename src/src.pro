TEMPLATE = subdirs
include(../config.pri)


CONFIG += ordered
message($$LIBS)

SUBDIRS += \
						 common \	
             json \
             http_requests \
             service \
      	     common/tst\
# GT-1073             json/tst\ 
#            service/tst
#	     maps \
#            http_requests/tst_http_requests \
#            tracker/clients/meego/tst \
