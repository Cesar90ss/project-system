NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="Read Only exception 3 0"

OUTPUT=$(./nachos-step4 -rs 2 -x trigger_page_fault | grep -E "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
