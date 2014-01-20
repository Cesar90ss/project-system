#!/bin/bash
NACHOS_DIR=../build/

cd $NACHOS_DIR
OUTPUT=`./nachos-step4 -rs 1 -x step4_multiple_ForkExec | head -n -10`
SORTED_STRING=$(echo "$OUTPUT" | sort | uniq -c)
echo "$SORTED_STRING"


if [ -z "$(echo "$SORTED_STRING" | grep -i "12 Child ending")" ]; then	#12 process created
	exit -1
elif [ -z "$(echo "$SORTED_STRING" | grep -i "Parent ending")" ]; then	#1 parent
	exit -2
elif [ -z "$(echo "$SORTED_STRING" | grep -i "144 I am a thread in a process")" ]; then	#12 thread for each proc(total 144)
	exit -3
fi
	exit 0
