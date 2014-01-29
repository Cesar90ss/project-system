NACHOS_DIR=../build/

cd $NACHOS_DIR
RESULT_STRING=""
for i in {1..13}; do
    RESULT_STRING="$RESULT_STRING$i"
done

./nachos-final -cp malloc_free_multiple run
OUTPUT=$(./nachos-final -rs 5 -x run | tr -d "\n" | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
