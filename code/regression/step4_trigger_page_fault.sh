NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="Read Only exception 3 0"

./nachos-final -cp trigger_page_fault run
OUTPUT=$(./nachos-final -rs 2 -x run | grep -E "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
