NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f

#create file structure
echo -n "1" > /tmp/test1
./nachos-step5 -cp /tmp/test1 test
./nachos-step5 -cp step5_two_threads_opening_file run

# Test case
OUTPUT=$(./nachos-step5 -x -rs 4 run | grep -E "a*ba*ba*")

#Clean up test
./nachos-step5 -f

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
