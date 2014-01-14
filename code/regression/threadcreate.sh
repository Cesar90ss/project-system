NACHOS_DIR=../build/

cd $NACHOS_DIR


RESULT_STRING="NewThread"


OUTPUT=$( ./nachos-step3  -rs2 -d m -x threadcreate| grep -P $RESULT_STRING)

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
