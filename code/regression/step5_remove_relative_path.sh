NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f 

#create file structure
./nachos-step5 -md a
./nachos-step5 -md a/aa
./nachos-step5 -md a/ab
./nachos-step5 -md a/aa/aaa
./nachos-step5 -md a/aa/aab
./nachos-step5 -md a/ab/aba

./nachos-step5 -rd a/aa/aaa

# Test case
OUTPUT=$(./nachos-step5 -l a/aa | head -n -10 | tr "\n" "|")
EXPECTED_OUTPUT="d - aab|"

if [ "$OUTPUT" = "$EXPECTED_OUTPUT" ]; then
    RETURN=0
else
    RETURN=-1
fi

#Clean up test
./nachos-step5 -f

#return test result
exit $RETURN
