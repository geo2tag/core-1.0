#!/bin/sh

package_name=`./pkg_package_name.sh`

rm -rf ./BUILD
mkdir -p BUILD/${package_name}

is_dirty=`git status ./src/ -s`

if [ "${is_dirty}"  = "" ]; then
	echo "src is clean"
else
	echo "src is dirty"
	exit 1
fi


