#!/bin/bash
NACHOS_DIR=../build/

cd $NACHOS_DIR
OUTPUT=`./nachos-step4 -rs 1 -x step4_multiple_ForkExec | head -n -10`
SORTED_STRING=$(echo "$OUTPUT" | sort | uniq -c)
echo "$SORTED_STRING"


if [ -z "$(echo "$SORTED_STRING" | grep -i "15 Child ending")" ]; then
	exit -1
elif [ -z "$(echo "$SORTED_STRING" | grep -i "Parent ending")" ]; then
	exit -2
fi
	exit 0
