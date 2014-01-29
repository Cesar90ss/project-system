NACHOS_DIR=../build/

cd $NACHOS_DIR
OUTPUT=`./nachos-step6 -m 1 -x step6_listen_existing_port`

if [ -n "$(echo "$OUTPUT" | grep "Invalid Listening Port")" ]; then	
	exit 0 
else
	exit -1
fi

