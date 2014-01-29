NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f


./nachos-final -cp create_file run

# Test case
./nachos-final -x run -rs 4
./nachos-final -l /
OUTPUT=$(./nachos-final -l / | tr "\n" "|" | grep "f - run|f - 42|")

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
