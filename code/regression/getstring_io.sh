NACHOS_DIR=../build/

cd $NACHOS_DIR

TEST_STRING="0123456789"
RESULT_STRING="01234 56789"


./nachos-final -cp getstring run
OUTPUT=$(echo $TEST_STRING | ./nachos-final -x run | grep -P "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
