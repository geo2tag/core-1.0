#!/bin/bash
#
# this file running tests.
#

export LD_LIBRARY_PATH=./debian/build/lib/


TESTS=`find ./ -name utest.*`

for test in ${TESTS}; 
do
	echo ${test} >>tests.list
	${test} -xml  | grep -v "?xml version=" >>tests.output
done;

echo '<?xml version="1.0" encoding="ISO-8859-1"?>' >tests.output.xml
cat ./tests.output >>tests.output.xml

