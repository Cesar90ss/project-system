NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="([01][^01]*){6}"

OUTPUT=$(./nachos-step3 -rs 2 -x threadJoinMultipleOk | grep -P "$RESULT_STRING" | grep "Parent finish")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi 
