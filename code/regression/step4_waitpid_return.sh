NACHOS_DIR=../build/

cd $NACHOS_DIR
RESULT_STRING="Child exit code was 42"

./nachos-final -cp waitpid_return_parent run
./nachos-final -cp waitpid_return run2
OUTPUT=$(./nachos-final -rs 5 -x run | grep "$RESULT_STRING")
if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
