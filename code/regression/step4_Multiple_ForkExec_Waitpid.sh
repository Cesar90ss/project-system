NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="in my world the parent should come after the child I am the child"

OUTPUT=$(./nachos-step4 -rs 0 -x step4_Multiple_ForkExec_Waitpid)

OUTPUT1=$(echo $OUTPUT | grep "$RESULT_STRING")



if [ -n "$OUTPUT1" ]; then
    exit -1
else
    exit 0
fi
