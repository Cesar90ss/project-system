NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="I can not wait for myself"

OUTPUT=$(./nachos-step4 -rs 0 -x step4_thread_Join_0)

OUTPUT1=$(echo $OUTPUT | grep "$RESULT_STRING")



if [ -n "$OUTPUT1" ]; then
    exit 0
else
    exit -1
fi
