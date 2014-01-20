NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="Read Only exception 3 0 ok"
./nachos-step4 -x ./trigger_page_fault_multiple_process -rs 2
OUTPUT=$(./nachos-step4 -x ./trigger_page_fault_multiple_process -rs 2 | tr "\n" " " | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
