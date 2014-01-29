NACHOS_DIR=../build/

cd $NACHOS_DIR
OUTPUT=`./nachos-step6 -m 1 -x step6_send_message_not_connected`


if [ -n "$(echo "$OUTPUT" | grep " Error, Socket not connected")" ]; then	
	exit 0 
else
	exit -1
fi

