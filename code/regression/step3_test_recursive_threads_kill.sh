#!/bin/bash

NACHOS_DIR=../build/
cd $NACHOS_DIR

RESULT_STRING="We are in the main thread We are in the child thread"

./nachos-final -cp test_recursive_threads_kill run
OUTPUT=$(./nachos-final -rs 2  -x run | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
	exit 0
else
	exit -1
fi
