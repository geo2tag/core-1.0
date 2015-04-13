#!/bin/bash

NUM=100000000

for ((i=0;i<${NUM}; i++)); do
echo "TEST:$i/${NUM}"
./single.sh localhost:80
done
