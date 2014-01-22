#!/bin/bash
NACHOS_DIR=../build/

cd $NACHOS_DIR
OUTPUT=`./nachos-step4 -rs 2 -x thread_max_limit | head -n -10`
SORTED_STRING=$(echo "$OUTPUT" | sort | uniq -c )
echo "$SORTED_STRING"

CREATED_THREAD=$(echo "$SORTED_STRING" | grep -i "Thread creation successfull" | awk '{ print $1 }')
EXECUTED_THREAD=$(echo "$SORTED_STRING" | grep -i "Thread execution" | awk '{ print $1 }' )
FAILED_THREAD=$(echo "$SORTED_STRING" | grep -i "Thread creation failed" | awk '{ print $1 }' )

if [ $CREATED_THREAD != $EXECUTED_THREAD ]; then
	echo "incoherent thread number"
	exit -1
elif [ -z "$(echo "$OUTPUT" | grep -i "Thread creation failed" )" ]; then
	echo "no process creation failed"
	exit -2
else
	exit 0
fi
