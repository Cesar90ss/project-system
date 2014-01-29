NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="La Fin !"

./nachos-final -cp thread_main_userthreadexit run
OUTPUT=$(./nachos-final -x run -rs 2 | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
