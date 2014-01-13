#!/bin/bash
#

NACHOS_DIR=../build/

cd $NACHOS_DIR

TEST_STRING="245678923456783456734567365678984558757"
OUTPUT_STRING="2147483647"

OUTPUT=$(echo "$TEST_STRING" | ./nachos-step2 -x getint | grep "$OUTPUT_STRING")
if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
