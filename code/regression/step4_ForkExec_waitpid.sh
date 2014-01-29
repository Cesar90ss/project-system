NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="I am the child, in my world the parent should come after the child..."

./nachos-final -cp step4_ForkExec_Waitpid run
./nachos-final -cp step4_LoopForWaitpid run2

OUTPUT=$(./nachos-final -rs 5 -x run)

OUTPUT1=$(echo $OUTPUT | grep "$RESULT_STRING")



if [ -n "$OUTPUT1" ]; then
    exit 0
fi

exit -1

