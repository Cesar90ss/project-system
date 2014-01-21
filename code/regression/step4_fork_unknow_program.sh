NACHOS_DIR=../build/

cd $NACHOS_DIR
RESULT_STRING="Machine Halting"

OUTPUT=$(./nachos-step4 -rs 5 -x fork_unknow_program | grep -c "$RESULT_STRING" )

if [ -n $OUTPUT ]; then
    exit 0
else
    exit -1
fi
