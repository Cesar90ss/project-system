NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f

#create file structure
./nachos-final -md a
./nachos-final -md a/aa
./nachos-final -md a/ab
./nachos-final -cp ../Makefile a/aa/Makefile
./nachos-final -md b
./nachos-final -md b/ba
./nachos-final -md b/bb

# Test case
./nachos-final -lr /
OUTPUT=$(./nachos-final -lr / | head -n -10 | tr "\n" "|")
EXPECTED_OUTPUT="/a|/a/aa|/a/aa/Makefile|/a/ab|/b|/b/ba|/b/bb|"

if [ "$OUTPUT" = "$EXPECTED_OUTPUT" ]; then
    RETURN=0
else
    RETURN=-1
fi

#Clean up test
./nachos-final -f

#return test result
exit $RETURN
