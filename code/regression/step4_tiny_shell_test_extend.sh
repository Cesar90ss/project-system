#!/bin/bash
NACHOS_DIR=../build/

cd $NACHOS_DIR

./nachos-final -cp step4_tiny_shell_test_program1 run1
./nachos-final -cp step4_tiny_shell_test_program2 run2
./nachos-final -cp step4_tiny_shell run

OUTPUT=$(printf "run1\nrun2 &\nfg 0\n" | ./nachos-final -x run -rs 2)
SORTED_STRING=$(echo "$OUTPUT" | sort | uniq -c)
echo "$SORTED_STRING"

if [ -z '$(echo "$SORTED_STRING" | grep -i "Program 1 is Running")' ]; then
    exit -1
elif [ -z '$(echo "$SORTED_STRING" | grep -i "The Job was launched in background with id : 0")' ]; then
    exit -2
elif [ -z '$(echo "$SORTED_STRING" | grep -i "fg : no such job")' ]; then
    exit -3
fi
    exit 0
