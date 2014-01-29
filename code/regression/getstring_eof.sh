NACHOS_DIR=../build/

cd $NACHOS_DIR

TEST_STRING="ABC"
RESULT_STRING="ABC"

./nachos-final -cp getstringeof run
OUTPUT=$(echo $TEST_STRING | ./nachos-final -x run | grep $RESULT_STRING)

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
