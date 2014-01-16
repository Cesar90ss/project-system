#!/bin/bash
NACHOS_DIR=../build/

cd $NACHOS_DIR
OUTPUT=`./nachos-step3 -rs 4 -x step3_use_destroyed_semaphore | head -n -10`
STRING=$( echo $OUTPUT | grep "error" )

if [ -z $STRING  ]; then
	exit 0
else
	exit -1
fi