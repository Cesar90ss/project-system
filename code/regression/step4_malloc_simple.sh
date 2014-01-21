NACHOS_DIR=../build/

cd $NACHOS_DIR
RESULT_STRING=""
for i in {0..99}; do
    RESULT_STRING="$RESULT_STRING$i "
done

OUTPUT=$(./nachos-step4 -rs 5 -x malloc_simple_test | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
