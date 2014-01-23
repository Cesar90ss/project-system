NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f 

#create file structure
./nachos-step5 -md a
./nachos-step5 -md a/aa
./nachos-step5 -md a/ab
./nachos-step5 -cp a/aa/Makefile
./nachos-step5 -md b
./nachos-step5 -md b/ba
./nachos-step5 -md b/bb

# Test case
OUTPUT=$(./nachos-step5 -lr / | head -n -10 | tr "\n" "|")
EXPECTED_OUTPUT="a|a/aa|a/aa/Makefile|b|b/ba|b/bb"

if [ "$OUTPUT" = "$EXPECTED_OUTPUT" ]; then
    RETURN=0
else
    RETURN=-1
fi

#Clean up test
./nachos-step5 -f

#return test result
exit $RETURN
