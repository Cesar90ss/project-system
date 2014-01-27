NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f

#create file structure
echo -n "1" > /tmp/test1

./nachos-step5 -cp /tmp/test1 test

./nachos-step5 -cp step5_remove_opened_file

# Test case
OUTPUT=$(./nachos-step5 -x run | grep "True")

./nachos-step5 -f

if [ -n "$OUTPUT" ]; then
    return 0
else
    return -1
fi

#Clean up test

#return test result
