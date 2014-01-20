NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="42 ok1 ok2 ok3"

OUTPUT=$(./nachos-step4 -rs 2 -x allocheappage_syscall | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
