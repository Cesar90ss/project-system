NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f

#create file structure
echo -n "1" > /tmp/test1
echo -n "2" > /tmp/test2

./nachos-final -cp /tmp/test1 test
./nachos-final -md a
./nachos-final -cp /tmp/test2 a/test
./nachos-final -cp multiple_file_open run

# Test case
./nachos-final -x run -rs 4
OUTPUT=$(./nachos-final -x run -rs 4 | grep "0112")

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
