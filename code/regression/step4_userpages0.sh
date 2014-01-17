NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="[a-Z]{27}"

OUTPUT1=$(./nachos-step4 -rs 2 -x userpages0 | grep -E "$RESULT_STRING" | cut -d" " -f1)

OUTPUT2=$(./nachos-step4 -rs 5 -x userpages0 | grep -E "$RESULT_STRING" | cut -d" " -f1)

if [ "$OUTPUT1" != "$OUTPUT2" ]; then
    exit 0
else
    exit -1
fi 
