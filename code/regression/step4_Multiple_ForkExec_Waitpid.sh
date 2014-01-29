NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="in my world the parent should come after the child Machine halting!"


./nachos-final -cp step4_Multiple_ForkExec_Waitpid run
./nachos-final -cp step4_LoopForWaitpid run2
OUTPUT=$(./nachos-final -rs 0 -x run | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
