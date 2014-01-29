NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f 

#create file structure
./nachos-final -md a
./nachos-final -md a/aa
./nachos-final -md a/ab
./nachos-final -md a/aa/aaa
./nachos-final -md a/aa/aab
./nachos-final -md a/ab/aba

./nachos-final -rd a/aa/aaa

# Test case
OUTPUT=$(./nachos-final -l a/aa | head -n -10 | tr "\n" "|")
EXPECTED_OUTPUT="d - aab|"

if [ "$OUTPUT" = "$EXPECTED_OUTPUT" ]; then
    RETURN=0
else
    RETURN=-1
fi

#Clean up test
./nachos-final -f

#return test result
exit $RETURN
