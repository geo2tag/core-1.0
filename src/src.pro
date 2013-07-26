TEMPLATE = subdirs
include(../config.pri)


CONFIG += ordered
message($$LIBS)

  SUBDIRS += common
  SUBDIRS += \
             json \
             http_requests \
             service \
	     common/tst\
#             json/tst 
#            service/tst
#            tracker/observer
#	     maps \
#            http_requests/tst_http_requests \
#            tracker/clients/meego/tst \
