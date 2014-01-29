NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="ok"

./nachos-final -cp thread_multiple_join run
OUTPUT=$(./nachos-final -x run -rs 2 | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
