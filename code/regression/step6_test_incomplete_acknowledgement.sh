NACHOS_DIR=../build/

cd $NACHOS_DIR
OUTPUT=`./nachos-step6 -m 0 -./nachos-step6 -m 0 -x step6_catch_error_server-4 & ./nachos-step6 -m 0 -./nachos-step6 -m 1 -x step6_catch_error_client-2`

if [ -n "$(echo "$OUTPUT" | grep "The Acknoledgement was not received by the client")" ]; then	
	exit 0 
else
	exit -1
fi

