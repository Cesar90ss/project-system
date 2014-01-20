NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="ok done ok"

OUTPUT=$(./nachos-step4 -rs 2 -x process_preempt | tr "\n" " " | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
