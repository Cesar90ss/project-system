NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f

#create file structure
./nachos-final -cp ../Makefile a
./nachos-final -md dir1
./nachos-final -md dir1/dir2
./nachos-final -cp ../Makefile dir1/test
./nachos-final -md dir3
./nachos-final -md dir3/dir4
./nachos-final -md dir3/dir4/dir5

# Test case
./nachos-final -l /dir3/../dir2/../dir3/dir4/./
OUTPUT=$(./nachos-final -l /dir3/../dir2/../dir3/dir4/./ | head -n -10 | tr "\n" "|")
EXPECTED_OUTPUT="d - dir5|"

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
