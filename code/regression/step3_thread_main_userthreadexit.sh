NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="La Fin !"

OUTPUT=$(./nachos-step4 -x thread_main_userthreadexit -rs 2 | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
