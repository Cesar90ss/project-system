NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f

./nachos-step5 -cp ../Makefile test

./nachos-step5 -cp step5_two_process_opening_file run
./nachos-step5 -cp step5_two_process_open_file_sub run2

# Test case

OUTPUT=$(./nachos-step5 -x run -rs 4 | grep -E "okok")

if [ -n "$OUTPUT" ]; then
    RETURN=0
else
    echo "$OUTPUT"
    RETURN=-1
fi

#Clean up test
#./nachos-step5 -f

#return test result
exit $RETURN
