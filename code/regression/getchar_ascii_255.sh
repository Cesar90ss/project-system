#!/bin/bash

NACHOS_DIR=../build/

cd $NACHOS_DIR
TEST_STRING="012345\xFF789"

./nachos-final -cp getchar run
OUTPUT=$(printf $TEST_STRING | ./nachos-final -x run | grep -P $TEST_STRING)

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
