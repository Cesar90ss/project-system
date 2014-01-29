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
./nachos-final -cp step5_absolute_path run

# Test case
OUTPUT=$(./nachos-final -x run | grep "1212")

#Clean up test
./nachos-final -f

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
