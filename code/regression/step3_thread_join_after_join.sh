NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="ok"

OUTPUT=$(./nachos-step3 -rs 2  -x thread_join_after_join | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
