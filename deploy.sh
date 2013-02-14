#!/bin/sh


ssh root@demo64.geo2tag.org /etc/init.d/lighttpd stop
scp ./src/service/fcgi_server root@demo64.geo2tag.org:/opt/geo2tag/
scp ./src/lib/lib*            root@demo64.geo2tag.org:/usr/lib/
ssh root@demo64.geo2tag.org /etc/init.d/lighttpd start

