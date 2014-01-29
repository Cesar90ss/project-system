NACHOS_DIR=../build/

cd $NACHOS_DIR
./nachos-final -cp step6_multiple_listen_same_port run
OUTPUT=`./nachos-final -m 1 -x run`


if [ -n "$(echo "$OUTPUT" | grep " The port is already used by another listener")" ]; then	
	exit 0 
else
	exit -1
fi

