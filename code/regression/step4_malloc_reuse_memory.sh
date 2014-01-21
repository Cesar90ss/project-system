NACHOS_DIR=../build/

cd $NACHOS_DIR
RESULT_STRING="ok"

OUTPUT=$(./nachos-step4 -rs 5 -x malloc_reuse_memory | grep "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
