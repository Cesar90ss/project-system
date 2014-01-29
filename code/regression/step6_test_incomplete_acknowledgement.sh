NACHOS_DIR=../build/

cd $NACHOS_DIR
./nachos-final -cp step6_catch_error_server-4 run
./nachos-final -cp step6_catch_error_client-2 run2

./nachos-final -m 0 -x run&
gdb --args ./nachos-step6 -m 1 -x run2

#OUTPUT=$(./nachos-final -m 0 -x run& ./nachos-step6 -m 1 -x run2)
C=$(echo "$OUTPUT" | grep "The Acknoledgement was not received by the client")
if [ -n "$C" ]; then
    exit 0
else
    exit -1
fi
