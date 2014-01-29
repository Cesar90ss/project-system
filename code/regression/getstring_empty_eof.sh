NACHOS_DIR=../build/

cd $NACHOS_DIR

TEST_STRING=""
RESULT_STRING="End Of File"

./nachos-final -cp getstringeof run
OUTPUT=$(echo -n $TEST_STRING | ./nachos-final -x run | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
