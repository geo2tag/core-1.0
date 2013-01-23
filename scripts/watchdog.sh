#!/bin/sh

while :
do
	
	str=`wget --quiet --timeout=0.3 --tries=1 http://127.0.0.1:80/service/version -O - |grep '"errno" : 0'`

	result=`echo ${str} | grep 0 |  wc -l`

	echo "str=${str}"
	echo "result=${result}"
	
	if [ "${result}" = "1" ] 
	then
		echo "ok"
	else
		echo "fail"
		/etc/init.d/lighttpd stop
		/etc/init.d/lighttpd start
		sleep 3
	fi
done
