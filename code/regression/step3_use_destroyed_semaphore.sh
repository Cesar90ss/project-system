#!/bin/bash
NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="ok"

OUTPUT=$(./nachos-step4 -rs 4 -x step3_use_destroyed_semaphore | head -n -10 | grep "$RESULT_STRING" )

if [ -n "$OUTPUT" ]; then
	exit 0
else
	exit -1
fi
