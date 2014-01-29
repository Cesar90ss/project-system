NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f

#create file structure
echo -n "test1" > /tmp/test1
echo -n "test2" > /tmp/test2

./nachos-final -cp /tmp/test1 test
./nachos-final -md a
./nachos-final -cp /tmp/test2 a/test

# Test case
./nachos-final -tk1

OUTPUT=$(echo $?)

#Clean up test
./nachos-final -f

if [ "$OUTPUT" -eq 0 ]; then
    exit 0
else
    exit -1
fi
