NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="La Fin!"

OUTPUT=$(./nachos-step3 -x thread_main_userthreadexit | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
