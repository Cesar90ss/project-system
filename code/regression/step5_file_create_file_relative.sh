NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f

#create file structure
./nachos-final -cp ../Makefile a

# Test case

# Make directory
./nachos-final -md dir
./nachos-final -md dir/dir2
./nachos-final -cp ../Makefile dir/dir2/dir3/.././../../dir/dir2/dir3

OUTPUT=$(./nachos-final -l /dir/dir2 | head -n -10 | tr "\n" "|")
EXPECTED_OUTPUT="f - dir3|"

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
