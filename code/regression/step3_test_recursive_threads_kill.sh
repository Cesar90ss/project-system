#!/bin/bash

NACHOS_DIR=../build/
cd $NACHOS_DIR

RESULT_STRING="We are in the main thread We are in the child thread"

OUTPUT=$(./nachos-step3 -rs 2  -x test_recursive_threads_kill | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
	exit 0
else
	exit -1
fi
