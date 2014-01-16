NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="(0{20}-3)"

OUTPUT=$(./nachos-step3 -rs 2 -x thread_max_limit | grep -P "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi 
