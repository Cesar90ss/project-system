NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="Join Successful !"

OUTPUT=$(./nachos-step3 -x threadjoin | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
