NACHOS_DIR=../build/

cd $NACHOS_DIR
./nachos-final -cp step6_test_accept_failed run
OUTPUT=`./nachos-final -m 1 -x run `


if [ -n "$(echo "$OUTPUT" | grep "No listening socket, can not accept anything")" ]; then	
	exit 0 
else
	exit -1
fi

