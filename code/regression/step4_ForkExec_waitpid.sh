NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="I am the child, in my world the parent should come after the child..."

OUTPUT=$(./nachos-step4 -rs 5 -x step4_ForkExec_Waitpid)

OUTPUT1=$(echo $OUTPUT | grep "$RESULT_STRING")



if [ -n "$OUTPUT1" ]; then
    exit 0
fi
done
    exit -1
 
