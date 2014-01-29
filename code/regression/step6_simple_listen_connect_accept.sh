NACHOS_DIR=../build/

cd $NACHOS_DIR

./nachos-final -cp step6_test_simple_listen_accept run
./nachos-final -cp step6_test_simple_listen_connect run2
for i in {1..3}
do
OUTPUT=`./nachos-final -m 0 -rs 2 -x run & ./nachos-final -m 1 -rs 4 -x run2`

if [ -n "$(echo "$OUTPUT" | grep "Received message : Got it!")" ] && [ -n "$(echo "$OUTPUT" | grep "Received message : Hello there!")" ]; then	
	exit 0 
fi
done
	exit -1


