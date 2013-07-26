#!/bin/bash
#
# this file running tests.
#

export LD_LIBRARY_PATH=./debian/build/lib/


TESTS=`find ./ -name utest.*`

echo "Found `echo ${TESTS} | wc -l` tests"

for test in ${TESTS}; 
do
	echo ${test}
	${test} -xml 
done;
