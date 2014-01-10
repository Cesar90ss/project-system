#!/bin/bash

NACHOS_DIR=../build/

cd $NACHOS_DIR

LONG_STRING=""

for i in {1..128}
do
	LONG_STRING=$LONG_STRING"a"
done
LONG_STRING=$LONG_STRING" "
for i in {1..128}
do
	LONG_STRING=$LONG_STRING"b"
done

echo $LONG_STRING
OUTPUT=$(./nachos-step2 -x longstring | grep "$LONG_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi

