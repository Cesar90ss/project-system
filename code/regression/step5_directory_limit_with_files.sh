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
./nachos-final -cp ../Makefile file3
./nachos-final -md dir4
./nachos-final -cp ../Makefile file5
./nachos-final -md dir6
./nachos-final -cp ../Makefile file7
./nachos-final -md dir8
./nachos-final -cp ../Makefile file9
./nachos-final -md dir10
./nachos-final -cp ../Makefile file11
./nachos-final -md dir12

OUTPUT=$(./nachos-final -l / | head -n -10 | tr "\n" "|")
EXPECTED_OUTPUT="f - a|d - dir1|d - dir2|f - file3|d - dir4|f - file5|d - dir6|f - file7|d - dir8|f - file9|"

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
