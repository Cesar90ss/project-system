NACHOS_DIR=../build/

cd $NACHOS_DIR
RESULT_STRING="Machine halting!"

./nachos-step4 -rs 5 -x fork_unknow_program

OUTPUT=$(./nachos-step4 -rs 5 -x fork_unknow_program | grep "$RESULT_STRING" )

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
