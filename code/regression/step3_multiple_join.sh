NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="ok"

OUTPUT=$(./nachos-step3 -x thread_multiple_join -rs 2 | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
