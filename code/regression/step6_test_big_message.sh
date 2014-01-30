NACHOS_DIR=../build/

cd $NACHOS_DIR
./nachos-final -f
./nachos-final -cp step6_test_big_message_server run
./nachos-final -cp step6_test_big_message_client run2

./nachos-final -m 0 -x run&

OUTPUT=$(./nachos-final -m 1 -x run2)
C=$(echo "$OUTPUT" | grep "Message Arrived")
if [ -n "$C" ]; then
    exit 0
else
    exit -1
fi
