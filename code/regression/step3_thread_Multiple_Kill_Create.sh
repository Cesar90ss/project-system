NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="Parent finish"

OUTPUT=$(./nachos-step4 -rs 2 -x thread_Multiple_Kill_Create | grep -P "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi 
