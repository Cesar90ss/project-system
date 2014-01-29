NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f 

#create file structure
./nachos-final -md a
./nachos-final -md b
./nachos-final -md c

./nachos-final -rd /

# Test case
OUTPUT=$(./nachos-final -l / | head -n -10 | tr "\n" "|")
EXPECTED_OUTPUT="d - a|d - b|d - c|"

if [ "$OUTPUT" = "$EXPECTED_OUTPUT" ]; then
    RETURN=0
else
    RETURN=-1
fi

#Clean up test
./nachos-final -f

#return test result
exit $RETURN
