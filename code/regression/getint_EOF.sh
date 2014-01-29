#!/bin/bash

NACHOS_DIR=../build/

cd $NACHOS_DIR

TEST_STRING=""

./nachos-final -cp getint run
OUTPUT=$(echo -n "$TEST_STRING" | ./nachos-final -x run | grep "EOF")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
