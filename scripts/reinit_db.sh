#!/bin/bash


/etc/init.d/lighttpd stop

sudo -u postgres createuser -s geo2tag
echo "sudo -u postgres psql --command=\"ALTER USER"
sudo -u postgres psql --command="ALTER USER geo2tag WITH PASSWORD 'geo2tag';"
sudo -u postgres psql --command="DROP DATABASE 'geo2tag';"

sudo -u postgres createdb -O geo2tag geo2tag
sudo -u postgres psql --command="GRANT ALL privileges on database geo2tag to geo2tag;"
psql geo2tag -U geo2tag < /opt/geo2tag/db/base.sql

/etc/init.d/lighttpd stop
/etc/init.d/postgresql restart
/etc/init.d/lighttpd start




