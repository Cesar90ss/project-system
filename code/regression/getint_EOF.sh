#!/bin/bash

NACHOS_DIR=../build/

cd $NACHOS_DIR

TEST_STRING=""

OUTPUT=$(echo -n "$TEST_STRING" | ./nachos-step2 -x getint | grep "EOF")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
