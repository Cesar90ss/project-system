NACHOS_DIR=../build/

cd $NACHOS_DIR
RESULT_STRING=""
for i in {1..10}; do
    RESULT_STRING="${RESULT_STRING}4950"
done

./nachos-final -cp malloc_multiple_process run
./nachos-final -cp malloc_simple_test_multiple run2
OUTPUT=$(./nachos-final -rs 5 -x run | grep "$RESULT_STRING" )

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
