#!/bin/bash
NACHOS_DIR=../build/

cd $NACHOS_DIR
OUTPUT=`./nachos-step4 -rs 4 -x threadSemaphore | head -n -10`
STRING=$( echo "$OUTPUT" | uniq )

if [ $STRING = "ABCDE" ]; then
	exit 0
else
	exit -1
fi
