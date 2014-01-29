NACHOS_DIR=../build/

cd $NACHOS_DIR
RESULT_STRING=""
for i in {0..99}; do
    RESULT_STRING="$RESULT_STRING$i "
done

./nachos-final -cp malloc_simple_test run
OUTPUT=$(./nachos-final -rs 5 -x run | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
