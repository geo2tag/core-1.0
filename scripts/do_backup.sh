#!/bin/bash
cd /opt/backup

free_space=`echo $(($(stat -f --format="%a*%S" .)))`
free_space_limit=104857600
echo "Free space geo2tag-dev $free_space"
echo "Limit		     $free_space_limit"
#104857600 - 100mb limit

# Check local free space
if (( $free_space < $free_space_limit )); then
  echo "Not enough space left on geo2tag-dev server"
  exit 1 
fi

sudo ./backup.sh
