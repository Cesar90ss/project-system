NACHOS_DIR=../build/

cd $NACHOS_DIR

for i in {1..5}
do
OUTPUT=`./nachos-step6 -m 0 -rs 5 -x step6_test_simple_listen_accept & ./nachos-step6 -m 1 -rs 3 -x step6_test_simple_listen_connect`

if [ -n "$(echo "$OUTPUT" | grep "Received message : Hello there!")" ] && [ -n "$(echo "$OUTPUT" | grep "Received message : Got it!")" ]; then	
	exit 0 
fi
done
	exit -1


