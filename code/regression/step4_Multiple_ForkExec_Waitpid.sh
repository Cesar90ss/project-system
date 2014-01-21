NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="in my world the parent should come after the child Machine halting!"


OUTPUT=$(./nachos-step4 -rs 0 -x step4_Multiple_ForkExec_Waitpid | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
