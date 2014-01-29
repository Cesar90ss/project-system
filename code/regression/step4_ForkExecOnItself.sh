NACHOS_DIR=../build/

cd $NACHOS_DIR



RESULT_STRING1="Error"			# Determine maximum thread created in the same time
RESULT_STRING2="Hello Myself"		# There is something thats execute
RESULT_STRING3="Machine halting!"	# No Segmentation fault

./nachos-final -cp step4_ForkExecOnItself run
OUTPUT=$(./nachos-final -x run -rs 3)

OUTPUT1=$(echo $OUTPUT | grep "$RESULT_STRING1")

OUTPUT2=$(echo $OUTPUT | grep "$RESULT_STRING2")

OUTPUT3=$(echo $OUTPUT | grep "$RESULT_STRING3")


if [ -n "$OUTPUT1" ] && [ -n "$OUTPUT2" ] && [ -n "$OUTPUT3" ]; then
    exit 0
else
    exit -1
fi
