NACHOS_DIR=../build/

cd $NACHOS_DIR
RESULT_STRING="ERROR : Ask for an invalid free address at 12"

./nachos-final -cp malloc_bad_free run
OUTPUT=$(./nachos-final -rs 5 -x run | grep "$RESULT_STRING")
if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
