NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f>/dev/null

#create file structure
./nachos-final -cp ../Makefile a >/dev/null
./nachos-final -md dir2>/dev/null
./nachos-final -md dir1>/dev/null
./nachos-final -md dir3>/dev/null

# Test case
OUTPUT=$(./nachos-final -l / | head -n -10 | tr "\n" "|")
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
./nachos-final -f>/dev/null

#return test result
exit $RETURN
