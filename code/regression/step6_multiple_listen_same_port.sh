NACHOS_DIR=../build/

cd $NACHOS_DIR
OUTPUT=`./nachos-step6 -m 1 -x step6_multiple_listen_same_port`


if [ -n "$(echo "$OUTPUT" | grep " The port is already used by another listener")" ]; then	
	exit 0 
else
	exit -1
fi

