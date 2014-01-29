NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f

#create file structure
./nachos-final -cp ../Makefile a

# Test case

# Make directory
./nachos-final -cp ../Makefile dir
./nachos-final -md dir

OUTPUT=$(./nachos-final -l / | head -n -10 | tr "\n" "|")
EXPECTED_OUTPUT="f - a|f - dir|"

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
