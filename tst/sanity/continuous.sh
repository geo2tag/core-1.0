#!/bin/bash

NUM=1000000

for ((i=0;i<${NUM}; i++)); do
echo "TEST:$i/${NUM}"
./single.sh
done
