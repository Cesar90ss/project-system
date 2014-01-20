NACHOS_DIR=../build/

cd $NACHOS_DIR

TEST_STRING=""
RESULT_STRING="End Of File"

OUTPUT=$(echo -n $TEST_STRING | ./nachos-step4 -x getstringeof | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
