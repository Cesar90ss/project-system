#!/bin/bash

NACHOS_DIR=../build/

cd $NACHOS_DIR
OUTPUT=$(./nachos-step2 -x halt | grep "Machine halting!")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
