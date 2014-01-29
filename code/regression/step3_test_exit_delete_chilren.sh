NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="Total thread actually running at end 1"

./nachos-final -cp thread_exit_delete_children run
OUTPUT=$(./nachos-final -rs 2 -x run | grep -P "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
