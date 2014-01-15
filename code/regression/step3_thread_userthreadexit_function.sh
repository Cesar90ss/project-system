NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="Exit code of child is 42"

OUTPUT=$(./nachos-step3 -rs 2  -x thread_userthreadexit_function | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
