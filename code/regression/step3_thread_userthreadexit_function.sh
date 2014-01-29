NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="Exit code of child is 42"

./nachos-final -cp thread_userthreadexit_function run
OUTPUT=$(./nachos-final -rs 2  -x run | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
