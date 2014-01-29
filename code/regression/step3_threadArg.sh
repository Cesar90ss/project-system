NACHOS_DIR=../build/

cd $NACHOS_DIR


RESULT_STRING="42"


./nachos-final -cp threadArg run
OUTPUT=$( ./nachos-final -rs 2 -d m -x run | grep -P $RESULT_STRING)

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
