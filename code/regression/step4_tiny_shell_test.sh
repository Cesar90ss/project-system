#!/bin/bash
NACHOS_DIR=../build/

cd $NACHOS_DIR

./nachos-final -cp step4_tiny_shell_test_program1 run1
./nachos-final -cp step4_tiny_shell_test_program2 run2
./nachos-final -cp step4_tiny_shell run3
OUTPUT=$(echo $'run1\run2\nhalt' | ./nachos-final -x run3 -rs 2)
SORTED_STRING=$(echo "$OUTPUT" | sort | uniq -c)
echo "$SORTED_STRING"

if [ -z '$(echo "$SORTED_STRING" | grep -i "Program 1 is Running")' ]; then
    exit -1
elif [ -z '$(echo "$SORTED_STRING" | grep -i "Program 2 is Running")' ]; then
    exit -2
elif [ -z '$(echo "$SORTED_STRING" | grep -i "Machine halting!")' ]; then
    exit -3
elif [ -z "$OUTPUT" ]; then
    exit -4
fi
exit 0
