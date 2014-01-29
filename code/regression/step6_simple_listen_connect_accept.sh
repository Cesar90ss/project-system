NACHOS_DIR=../build/

cd $NACHOS_DIR
./nachos-final -f
./nachos-final -cp step6_test_simple_listen_accept run
./nachos-final -cp step6_test_simple_listen_connect run2


for i in {1..10}
do
    OUTPUT=$(./nachos-final -m 0 -rs 2 -x run & ./nachos-final -m 1 -rs 4 -x run2)
    C1=$(echo "$OUTPUT" | grep "Received message : Got it!")
    C2=$(echo "$OUTPUT" | grep "Received message : Hello there!")
    echo $C1
    echo $C2
    if [ -n "$C1" ] && [ -n "$C2" ]; then
        exit 0
    fi
done
exit -1
