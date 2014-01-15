NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="Error while creating thread"

OUTPUT=$(./nachos-step3 -rs 2 -x thread_Multiple_Kill_Create | grep -P "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit -1
else
    exit 0
fi 
