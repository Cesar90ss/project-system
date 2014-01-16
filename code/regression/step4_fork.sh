#!/bin/bash
NACHOS_DIR=../build/

cd $NACHOS_DIR
OUTPUT=`./nachos-step4 -rs 2 -x step4_fork | head -n -10`
PARENT=$(echo "$OUTPUT" | grep parent)
CHILD=$(echo "$OUTPUT" | grep child)
echo "$PARENT"
echo "$CHILD"

if [ -z "$PARENT" ]; then
	exit -1
elif [ -z "$CHILD" ]; then
	exit -2
else
	exit 0
fi
