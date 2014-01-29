NACHOS_DIR=../build/

cd $NACHOS_DIR
./nachos-final -cp step6_send_message_not_connected run
OUTPUT=`./nachos-final -m 1 -x run `


if [ -n "$(echo "$OUTPUT" | grep " Error, Socket not connected")" ]; then	
	exit 0 
else
	exit -1
fi

