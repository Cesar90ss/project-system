#!/bin/bash
NACHOS_DIR=../build/

cd $NACHOS_DIR
OUTPUT=`./nachos-step4 -rs 2 -x thread_max_limit | head -n -10`
SORTED_STRING=$(echo "$OUTPUT" | sort | uniq -c )
echo "$SORTED_STRING"

CREATED_THREAD=$(echo "$SORTED_STRING" | grep -i "Thread creation successfull" | cut -c "6 7" )
EXECUTED_THREAD=$(echo "$SORTED_STRING" | grep -i "Thread execution" | cut -c "6 7" )
FAILED_THREAD=$(echo "$SORTED_STRING" | grep -i "Thread creation failed" | cut -c "5 6 7" )

if [ $CREATED_THREAD != $EXECUTED_THREAD ]; then
	echo "incoherent thread number"
	exit -1
elif [ -z "$(echo "$OUTPUT" | grep -i "Thread creation failed" )" ]; then
	echo "no process creation failed"
	exit -2
else
	exit 0
fi
