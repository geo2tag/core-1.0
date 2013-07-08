#!/bin/bash

# Usage: ./create_db.sh db_name
if [ $# -ne "1" ]
then
  echo "Usage: ./create_db.sh db_name"
  exit 1
fi

sudo -u postgres createdb -O geo2tag $1
psql $1 -U geo2tag < base.sql
