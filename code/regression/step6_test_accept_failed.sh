NACHOS_DIR=../build/

cd $NACHOS_DIR
OUTPUT=`./nachos-step6 -m 1 -x step6_test_accept_failed`


if [ -n "$(echo "$OUTPUT" | grep "No listening socket, can not accept anything")" ]; then	
	exit 0 
else
	exit -1
fi

