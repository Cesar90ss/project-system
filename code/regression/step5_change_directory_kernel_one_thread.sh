NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f

#create file structure
echo -n "test1" > /tmp/test1
echo -n "test2" > /tmp/test2

./nachos-step5 -cp /tmp/test1 test
./nachos-step5 -md a
./nachos-step5 -cp /tmp/test2 a/test

# Test case
./nachos-step5 -tk1

OUTPUT=$(echo $?)

#Clean up test
./nachos-step5 -f

if [ "$OUTPUT" -eq 0 ]; then
    exit 0
else
    exit -1
fi
