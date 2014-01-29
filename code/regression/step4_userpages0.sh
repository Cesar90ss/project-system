NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING="[a-Z]{27}"

./nachos-final -cp userpages0 run
OUTPUT1=$(./nachos-final -rs 2 -x run | grep -E "$RESULT_STRING" | cut -d" " -f1)

OUTPUT2=$(./nachos-final -rs 5 -x run | grep -E "$RESULT_STRING" | cut -d" " -f1)

if [ "$OUTPUT1" != "$OUTPUT2" ]; then
    exit 0
else
    exit -1
fi 
