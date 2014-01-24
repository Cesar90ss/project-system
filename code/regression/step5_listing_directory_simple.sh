NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f>/dev/null

#create file structure
./nachos-step5 -cp ../Makefile a >/dev/null
./nachos-step5 -md dir2>/dev/null
./nachos-step5 -md dir1>/dev/null
./nachos-step5 -md dir3>/dev/null

# Test case
OUTPUT=$(./nachos-step5 -l / | head -n -10 | tr "\n" "|")
EXPECTED_OUTPUT="f - a|d - dir2|d - dir1|d - dir3|"

echo "$OUTPUT"
echo "$EXPECTED_OUTPUT"
if [ "$OUTPUT" = "$EXPECTED_OUTPUT" ]; then
    RETURN=0
else
    echo "$OUTPUT"
    RETURN=-1
fi

#Clean up test
./nachos-step5 -f>/dev/null

#return test result
exit $RETURN
