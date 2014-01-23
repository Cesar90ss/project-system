NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f 

#create file structure
./nachos-step5 -md a
./nachos-step5 -md b
./nachos-step5 -md c

./nachos-step5 -rd a

# Test case
OUTPUT=$(./nachos-step5 -l | head -n -10 | tr "\n" "|")
EXPECTED_OUTPUT="d - b|d -c"

if [ "$OUTPUT" = "$EXPECTED_OUTPUT" ]; then
    RETURN=0
else
    RETURN=-1
fi

#Clean up test
./nachos-step5 -f

#return test result
exit $RETURN
