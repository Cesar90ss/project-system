NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="0123456789joined successfull"

OUTPUT=$(./nachos-step4 -rs 2 -x threadJoinSimple | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi

