#!/bin/bash

NACHOS_DIR=../build/

cd $NACHOS_DIR
./nachos-final -cp halt run
OUTPUT=$(./nachos-final -x run | grep "Machine halting!")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
