NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f

#create file structure
echo -n "1" > /tmp/test1
echo -n "2" > /tmp/test2

./nachos-step5 -cp /tmp/test1 test
./nachos-step5 -md a
./nachos-step5 -cp /tmp/test2 a/test

# Test case
./nachos-step5 -x change_directory_thread -rs 4
OUTPUT=$(./nachos-step5 -x change_directory_thread -rs 4 | grep "//a/11")

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
