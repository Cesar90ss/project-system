NACHOS_DIR=../build/

cd $NACHOS_DIR
OUTPUT=`./nachos-step6 -m 0 -x step6_test_simple_listen_accept & ./nachos-step6 -m 1 -x step6_test_simple_listen_connect`

if [ -n "$(echo "$OUTPUT" | grep "Received message : Got it!")" ] && [ -n "$(echo "$OUTPUT" | grep "Received message : Hello there!")" ]; then	
	exit 0 
else
	exit -1
fi

