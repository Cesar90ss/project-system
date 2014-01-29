NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="ok done ok"

./nachos-final -cp process_preempt run
./nachos-final -cp process_long_loop run2
OUTPUT=$(./nachos-final -rs 2 -x run | tr "\n" " " | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
