#!/bin/bash

NACHOS_DIR=../build/

cd $NACHOS_DIR
TEST_STRING="0123456789"

./nachos-final -cp getchar run
OUTPUT=$(echo $TEST_STRING | ./nachos-final -x run | grep $TEST_STRING)

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
