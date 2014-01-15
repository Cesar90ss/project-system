NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="Total thread actually running at end 1"

OUTPUT=$(./nachos-step3 -rs 2 -x thread_exit_delete_children | grep -P "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
