#!/bin/sh

scp ./src/service/fcgi_server root@demo64.geo2tag.org:/opt/geo2tag/
ssh root@demo64.geo2tag.org /etc/init.d/lighttpd restart

