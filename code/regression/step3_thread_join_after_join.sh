NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="ok"

./nachos-final -cp thread_join_after_join run
OUTPUT=$(./nachos-final -rs 2  -x run | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
