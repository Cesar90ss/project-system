NACHOS_DIR=../build/

cd $NACHOS_DIR

TEST_STRING="ABC"
RESULT_STRING="ABC"

OUTPUT=$(echo $TEST_STRING | ./nachos-step2 -x getstringeof | grep $RESULT_STRING)

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi