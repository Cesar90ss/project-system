NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f


./nachos-step5 -cp create_file run

# Test case
./nachos-step5 -x run -rs 4
./nachos-step5 -l /
OUTPUT=$(./nachos-step5 -l / | tr "\n" "|" | grep "f - run|f - 42|")

if [ -n "$OUTPUT" ]; then
    RETURN=0
else
    echo "$OUTPUT"
    RETURN=-1
fi

#Clean up test
./nachos-step5 -f

#return test result
exit $RETURN
