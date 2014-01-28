NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f

#create file structure
echo -n "1" > /tmp/test1
./nachos-step5 -cp /tmp/test1 test
./nachos-step5 -cp step5_two_threads_opening_file run

# Test case
./nachos-step5 -x run -rs 4
OUTPUT=$(./nachos-step5 -x run -rs 4 | grep -E "(ok){9}END")

#Clean up test
./nachos-step5 -f

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
