NACHOS_DIR=../build/

cd $NACHOS_DIR
RESULT_STRING=""
for i in {1..10}; do
    RESULT_STRING="${RESULT_STRING}4950"
done
./nachos-step4 -rs 5 -x malloc_multiple_process
OUTPUT=$(./nachos-step4 -rs 5 -x malloc_multiple_process | grep "$RESULT_STRING" )

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
