NACHOS_DIR=../build/

cd $NACHOS_DIR
RESULT_STRING="ERROR : Ask for an invalid free address at 12"

OUTPUT=$(./nachos-step4 -rs 5 -x malloc_bad_free | grep -c "$RESULT_STRING" )

if [ -n $OUTPUT ]; then
    exit 0
else
    exit -1
fi
