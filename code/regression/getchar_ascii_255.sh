#!/bin/bash

NACHOS_DIR=../build/

cd $NACHOS_DIR
TEST_STRING="012345\xFF789"

OUTPUT=$(printf $TEST_STRING | ./nachos-step4 -x getchar | grep -P $TEST_STRING)

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
