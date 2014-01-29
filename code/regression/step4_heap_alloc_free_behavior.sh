NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="42 ok1 ok2 ok3"

./nachos-final -cp allocheappage_syscall run
OUTPUT=$(./nachos-final -rs 2 -x run | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
