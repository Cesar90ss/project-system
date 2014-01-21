NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="No need to wait Dead Process "

OUTPUT=$(./nachos-step4 -rs 0 -x step4_ForkExec_Waitpid_DeadProcess)

OUTPUT1=$(echo $OUTPUT | grep "$RESULT_STRING")



if [ -n "$OUTPUT1" ]; then
    exit 0
else
    exit -1
fi
