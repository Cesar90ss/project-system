NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="Parent finish"

./nachos-final -cp thread_Multiple_Kill_Create run
OUTPUT=$(./nachos-final -rs 2 -x run | grep -P "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi 
