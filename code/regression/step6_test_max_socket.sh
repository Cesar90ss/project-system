NACHOS_DIR=../build/

cd $NACHOS_DIR
./nachos-final -cp step6_catch_error_server-3-4 run
./nachos-final -cp step6_catch_error_client-1 run2

./nachos-final -m 0 -x run > /tmp/nachos.output &
./nachos-final -m 1 -x run2 &
./nachos-final -m 2 -x run2 &
./nachos-final -m 3 -x run2 &
wait
OUTPUT=$(cat /tmp/nachos.output | grep "No More Free Socket")
echo $OUTPUT

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
