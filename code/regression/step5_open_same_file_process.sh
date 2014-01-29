NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f

./nachos-final -cp ../Makefile test

./nachos-final -cp step5_two_process_opening_file run
./nachos-final -cp step5_two_process_open_file_sub run2

# Test case

OUTPUT=$(./nachos-final -x run -rs 4 | grep -E "okok")

if [ -n "$OUTPUT" ]; then
    RETURN=0
else
    echo "$OUTPUT"
    RETURN=-1
fi

#Clean up test
./nachos-final -f

#return test result
exit $RETURN
