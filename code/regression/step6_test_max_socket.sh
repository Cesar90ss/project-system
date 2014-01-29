NACHOS_DIR=../build/

cd $NACHOS_DIR
OUTPUT=`./nachos-step6 -m 0 -x step6_catch_error_server-3-4 & ./nachos-step6 -m 1 -x step6_catch_error_client-1 && ./nachos-step6 -m 2 -x step6_catch_error_client-1`


if [ -n "$(echo "$OUTPUT" | grep "No More Free Socket")" ]; then	
	exit 0 
else
	exit -1
fi

