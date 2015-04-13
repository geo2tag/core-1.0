#!/bin/bash

# This script adds new user to the service db (specified as an argument) 
# Usage: ./add_user.sh login password email service_db_name

if [ $# -ne "4" ]
then
  echo "Usage: ./add_user.sh login password email service_db_name"
  exit 1
fi

login=$1;
password=$2;
email=$3;
db_name=$4;

config_file="/opt/geo2tag/geo2tag.conf";

# Steps: obtain default db name from config, add new user to default, add user to db_name
# 1
if [ ! -f $config_file ]
then
	echo "Config file ( $config_file ) does not exist."
	exit 1
fi

default_db_name=`cat $config_file | grep -o 'name=.*$' | grep -o '[^=]*$'`;

add_user_request="insert into users (login, password, email) values ('$login', '$password', '$email');";

#2
echo $add_user_request | psql $default_db_name geo2tag
#3
echo $add_user_request | psql $db_name geo2tag

echo "select * from users where login='$login';" | psql $default_db_name geo2tag
echo "select * from users where login='$login';" | psql $db_name geo2tag
