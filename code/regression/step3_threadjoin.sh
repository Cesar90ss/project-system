NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="Join Successful !"

./nachos-final -cp threadjoin run
OUTPUT=$(./nachos-final -rs 2  -x run | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
