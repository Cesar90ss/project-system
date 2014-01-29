NACHOS_DIR=../build/

cd $NACHOS_DIR
./nachos-final -cp step6_receive_message_waiting run
OUTPUT=`./nachos-final -m 1 -x run `


if [ -n "$(echo "$OUTPUT" | grep "Uncatched Error")" ]; then	
	exit 0 
else
	exit -1
fi

