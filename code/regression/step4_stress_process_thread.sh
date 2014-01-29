#!/bin/bash
NACHOS_DIR=../build/

cd $NACHOS_DIR


./nachos-final -cp step4_stress_process_thread_parent run
./nachos-final -cp step4_stress_process_thread_child run2
for i in {200..200}
do
	echo $i
	OUTPUT=`./nachos-final -rs $i -x run | head -n -10`
	SORTED_STRING=$(echo "$OUTPUT" | sort | uniq -c )
	echo "$SORTED_STRING"
	CREATED_CHILD=$(echo "$SORTED_STRING" | grep -i "Process creation successfull" | awk '{ print $1 }')
	STARTED_CHILD=$(echo "$SORTED_STRING" | grep -i "Child start" | awk '{ print $1 }')
	ENDED_CHILD=$(echo "$SORTED_STRING" | grep -i "Child end" | awk '{ print $1 }' )
#	let EXPECTED_FAILED_THREAD=$CREATED_CHILD*10


	CREATED_THREAD=$(echo "$SORTED_STRING" | grep -i "Thread creation successfull" | awk '{ print $1 }')
	EXECUTED_THREAD=$(echo "$SORTED_STRING" | grep -i "Thread execution" | awk '{ print $1 }')
#	FAILED_THREAD=$(echo "$SORTED_STRING" | grep -i "Thread creation failed" | awk '{ print $1 }')


	if [ $CREATED_CHILD != $STARTED_CHILD -o $STARTED_CHILD != $ENDED_CHILD ]; then
		echo "incoherent process number"
		exit -1
	elif [ $CREATED_THREAD != $EXECUTED_THREAD ]; then
		echo "incoherent thread number"
		exit -2
#	elif [ $EXPECTED_FAILED_THREAD -ne $FAILED_THREAD ]; then
#		echo "incoherent failed thread number"
#		exit -3
	elif [ -z "$(echo "$OUTPUT" | grep -i "Process creation failed" )" ]; then
		echo "no process creation failed"
		exit -4
	elif [ -z "$(echo "$OUTPUT" | grep -i "Parent end" )" ]; then
		exit -5
	fi
done
echo "OK"
exit 0
