#!/bin/bash

NACHOS_DIR=../build/

cd $NACHOS_DIR
TEST_STRING="abcd"

OUTPUT=$(printf $TEST_STRING | ./nachos-step2 -x putchar | grep $TEST_STRING)

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
