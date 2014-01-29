NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="Error while creating thread"

./nachos-final -cp threadJoinMultiple run
OUTPUT=$(./nachos-final -rs 2 -x run | grep -P "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi 
