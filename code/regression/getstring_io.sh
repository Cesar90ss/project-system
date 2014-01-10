NACHOS_DIR=../build/

cd $NACHOS_DIR

TEST_STRING="0123456789"
RESULT_STRING="01234 56789"


OUTPUT=$(echo $TEST_STRING | ./nachos-step2 -x getstring | grep -P "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
