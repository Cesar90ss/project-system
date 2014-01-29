NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f

#create file structure
./nachos-final -cp ../Makefile a

# Test case

# Make directory
./nachos-final -md dir1
./nachos-final -md dir2
./nachos-final -md dir3
./nachos-final -md dir4
./nachos-final -md dir5
./nachos-final -md dir6
./nachos-final -md dir7
./nachos-final -md dir8
./nachos-final -md dir9
./nachos-final -rd dir5
./nachos-final -rd dir6
./nachos-final -md dir10
./nachos-final -md dir11
./nachos-final -md dir12

OUTPUT=$(./nachos-final -l / | head -n -10 | tr "\n" "|")
EXPECTED_OUTPUT="f - a|d - dir1|d - dir2|d - dir3|d - dir4|d - dir10|d - dir11|d - dir7|d - dir8|d - dir9|"

if [ "$OUTPUT" = "$EXPECTED_OUTPUT" ]; then
    RETURN=0
else
    echo "$OUTPUT"
    RETURN=-1
fi

#Clean up test
./nachos-final -f

#return test result
exit $RETURN
