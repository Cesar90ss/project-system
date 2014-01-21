#!/bin/bash
NACHOS_DIR=../build/

cd $NACHOS_DIR

OUTPUT=$(echo $'step4_tiny_shell_test_program1\nstep4_tiny_shell_test_program2\nhalt' | ./nachos-step4 -x step4_tiny_shell -rs 2)
SORTED_STRING=$(echo "$OUTPUT" | sort | uniq -c)
echo "$SORTED_STRING"

if [ -z "$(echo "$SORTED_STRING" | grep -i "Program 1 is Running")" ]; then	
	exit -1
elif [ -z "$(echo "$SORTED_STRING" | grep -i "Program 2 is Running")" ]; then	
	exit -2
elif [ -z "$(echo "$SORTED_STRING" | grep -i "Machine halting!")" ]; then	
	exit -3
fi
	exit 0
