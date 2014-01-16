NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="Error !"

OUTPUT=$(./nachos-step4 -rs 2  -x threadjoinerror | grep -P "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
