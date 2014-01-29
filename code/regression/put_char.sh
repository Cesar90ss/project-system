#!/bin/bash

NACHOS_DIR=../build/

cd $NACHOS_DIR
TEST_STRING="abcd"

./nachos-final -cp putchar run
OUTPUT=$(printf $TEST_STRING | ./nachos-final -x run | grep $TEST_STRING)

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
