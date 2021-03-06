#!/bin/bash

NACHOS_DIR=../build/

cd $NACHOS_DIR

EXPECTED_OUTPUT="\-5 \-4 \-3 \-2 \-1 0 1 2 3 4 5 "

./nachos-final -cp putint run
OUTPUT=$(./nachos-final -x run | grep "$EXPECTED_OUTPUT")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
