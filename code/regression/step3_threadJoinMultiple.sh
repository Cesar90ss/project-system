NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="([01][^01]*){6}"

./nachos-final -cp threadJoinMultipleOk run
OUTPUT=$(./nachos-final -rs 2 -x run | grep -P "$RESULT_STRING" | grep "Parent finish")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
