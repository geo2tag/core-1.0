TEMPLATE = subdirs
include(../config.pri)


CONFIG += ordered
message($$LIBS)

  SUBDIRS += common
  SUBDIRS += \
             json \
             http_requests \
             service \
	     maps \ 
#            http_requests/tst_http_requests \
#             tracker/clients/meego/tst \
             #json/tst \
             #common/tst
             #service/tst
             #tracker/observer
