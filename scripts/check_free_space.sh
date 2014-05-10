#!/bin/bash
free_space=`echo $(($(stat -f --format="%a*%S" .)))`
free_space_limit=104857600
name=`uname -n`
echo "Server $name"
echo "Free space $free_space"
echo "Limit      $free_space_limit"
#104857600 - 100mb limit
# Check local free space
if (( $free_space < $free_space_limit )); then
  echo "Not enough space left on $name server"
  exit 1 
fi
