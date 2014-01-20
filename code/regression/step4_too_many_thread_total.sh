#!/bin/bash
NACHOS_DIR=../build/

cd $NACHOS_DIR
OUTPUT=`./nachos-step4 -rs 1 -x step4_too_many_thread_total | head -n -10`
SORTED_STRING=$(echo "$OUTPUT" | sort | uniq -c )
echo "$SORTED_STRING"

CREATED_THREAD=$(echo "$SORTED_STRING" | grep -i "Thread creation successfull" | cut -c "6 7" ) 
ENDED_THREAD=$(echo "$SORTED_STRING" | grep -i "I am a thread in a process" | cut -c "6 7"  )

if [ $CREATED_THREAD != $ENDED_THREAD ]; then
	exit -1
elif [ -z "$(echo "$OUTPUT" | grep -i "Thread creation failed" )" ]; then
	exit -2
else
	exit 0
fi
