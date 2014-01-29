NACHOS_DIR=../build/

cd $NACHOS_DIR
OUTPUT=`./nachos-step6 -m 1 -x step6_receive_message_waiting`


if [ -n "$(echo "$OUTPUT" | grep "Uncatched Error")" ]; then	
	exit 0 
else
	exit -1
fi

