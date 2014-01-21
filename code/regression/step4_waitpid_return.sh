NACHOS_DIR=../build/

cd $NACHOS_DIR
RESULT_STRING="Child exit code was 42"

OUTPUT=$(./nachos-step4 -rs 5 -x waitpid_return_parent | grep "$RESULT_STRING")
if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
