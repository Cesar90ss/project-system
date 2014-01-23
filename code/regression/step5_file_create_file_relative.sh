NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f

#create file structure
./nachos-step5 -cp ../Makefile a

# Test case

# Make directory
./nachos-step5 -md dir
./nachos-step5 -md dir/dir2
./nachos-step5 -cp ../Makefile dir/dir2/dir3/.././../dir/dir2/dir3

OUTPUT=$(./nachos-step5 -l/dir/dir2 | head -n -10 | tr "\n" "|")
EXPECTED_OUTPUT="f - dir3"

if [ "$OUTPUT" = "$EXPECTED_OUTPUT" ]; then
    RETURN=0
else
    echo "$OUTPUT"
    RETURN=-1
fi

#Clean up test
./nachos-step5 -f

#return test result
exit $RETURN
