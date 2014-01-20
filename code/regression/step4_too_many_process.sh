#!/bin/bash
NACHOS_DIR=../build/

cd $NACHOS_DIR
OUTPUT=`./nachos-step4 -rs 1 -x step4_too_many_process | head -n -10`
SORTED_STRING=$(echo "$OUTPUT" | sort | uniq -c )
echo "$SORTED_STRING"

CREATED_CHILD=$(echo "$SORTED_STRING" | grep -i "Creation successfull" | cut -c "6 7" ) 
ENDED_CHILD=$(echo "$SORTED_STRING" | grep -i "Child ending" | cut -c "6 7"  )

if [ $CREATED_CHILD != $ENDED_CHILD ]; then
	exit -1
elif [ -z "$(echo "$OUTPUT" | grep -i "Process creation failed" )" ]; then
	exit -2
else
	exit 0
fi
