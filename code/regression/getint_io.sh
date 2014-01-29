#!/bin/bash

NACHOS_DIR=../build/

cd $NACHOS_DIR

TEST_STRING="-5 -4 -3 -2 -1 0 1 2 3 4 5 "
OUTPUT_STRING="\-5 \-4 \-3 \-2 \-1 0 1 2 3 4 5 "

./nachos-final -cp getint run
OUTPUT=$(echo "$TEST_STRING" | ./nachos-final -x run | grep "$OUTPUT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
