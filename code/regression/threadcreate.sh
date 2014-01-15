NACHOS_DIR=../build/

cd $NACHOS_DIR


RESULT_STRING="Job done"
ERROR_STRING="Error"


OUTPUT=$( ./nachos-step3 -rs 2 -x threadcreate |grep -P "$ERROR_STRING")
if [ -n "$OUTPUT" ]; then
    exit -1
fi

OUTPUT=$( ./nachos-step3 -rs 2 -x threadcreate |grep -P "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -2
fi
