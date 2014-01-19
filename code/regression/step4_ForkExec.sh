NACHOS_DIR=../build/

cd $NACHOS_DIR

RESULT_STRING1="String2String1"
RESULT_STRING2="String1String2"

for i in {1..10}
do
OUTPUT=$(./nachos-step4 -rs 5 -x step4_ForkExec)

OUTPUT1=$(echo $OUTPUT | grep "$RESULT_STRING1")

OUTPUT2=$(echo $OUTPUT | grep "$RESULT_STRING2")



if [ -n "$OUTPUT1" ] && [ -n "$OUTPUT2" ]; then
    exit 0
fi
done
    exit -1
 
