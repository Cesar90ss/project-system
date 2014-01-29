NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="Read Only exception 3 0 ok"

./nachos-final -cp ./trigger_page_fault_multiple_process run
./nachos-final -cp ./trigger_page_fault run2
OUTPUT=$(./nachos-final -x run -rs 2 | tr "\n" " " | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
