NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="0123456789joined successfull"

./nachos-final -cp threadJoinSimple run
OUTPUT=$(./nachos-final -rs 2 -x run | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi

