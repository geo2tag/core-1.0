#!/bin/sh

branch=`git branch | grep "*" | cut -d' ' -f2`
package_name="`cat .version`.${branch}"

echo ${package_name}


