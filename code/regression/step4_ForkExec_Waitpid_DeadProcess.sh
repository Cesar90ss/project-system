NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="No need to wait Dead Process "

./nachos-final -cp step4_ForkExec_Waitpid_DeadProcess run
./nachos-final -cp step4_LoopForWaitpid run2
OUTPUT=$(./nachos-final -rs 0 -x run)



OUTPUT1=$(echo $OUTPUT | grep "$RESULT_STRING")



if [ -n "$OUTPUT1" ]; then
    exit 0
else
    exit -1
fi
