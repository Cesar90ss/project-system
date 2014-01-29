#!/bin/bash
NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="ok"

./nachos-final -cp step3_use_destroyed_semaphore run
OUTPUT=$(./nachos-final -rs 4 -x run | head -n -10 | grep "$RESULT_STRING" )

if [ -n "$OUTPUT" ]; then
	exit 0
else
	exit -1
fi
