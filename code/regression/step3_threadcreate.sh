NACHOS_DIR=../build/

cd $NACHOS_DIR


RESULT_STRING="Job done"
ERROR_STRING="Error"


./nachos-final -cp threadcreate run
OUTPUT=$( ./nachos-final -rs 2 -x run |grep -P "$ERROR_STRING")
if [ -n "$OUTPUT" ]; then
    exit -1
fi

OUTPUT=$( ./nachos-final -rs 2 -x run |grep -P "$RESULT_STRING")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -2
fi
