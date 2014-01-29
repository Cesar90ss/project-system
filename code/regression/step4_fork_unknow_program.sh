NACHOS_DIR=../build/

cd $NACHOS_DIR
RESULT_STRING="Machine halting!"


./nachos-final -cp fork_unknow_program run
OUTPUT=$(./nachos-final -rs 5 -x run | grep "$RESULT_STRING" )

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
