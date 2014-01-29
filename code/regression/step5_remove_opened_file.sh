NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f

#create file structure
echo -n "1" > /tmp/test1

./nachos-final -cp /tmp/test1 test

./nachos-final -cp step5_remove_opened_file run

# Test case
OUTPUT=$(./nachos-final -x run | grep "Didnt removed the file")

./nachos-final -f

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
