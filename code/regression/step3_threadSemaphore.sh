#!/bin/bash
NACHOS_DIR=../build/

cd $NACHOS_DIR
./nachos-final -cp threadSemaphore run
OUTPUT=`./nachos-final -rs 4 -x run | head -n -10`
STRING=$( echo "$OUTPUT" | uniq )

if [ $STRING = "ABCDE" ]; then
	exit 0
else
	exit -1
fi
