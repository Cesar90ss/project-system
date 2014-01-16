NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="Exit :42"

OUTPUT=$(./nachos-step4 -rs 2  -x thread_exit_code_wait_tool_late | grep "$RESULT_STRING")

 if [ -n "$OUTPUT" ]; then
     exit 0
 else
     exit -1
 fi
