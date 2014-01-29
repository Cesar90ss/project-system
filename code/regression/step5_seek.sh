NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f

echo "abcd" > /tmp/test
#copy c file in nachos
./nachos-final -cp /tmp/test test
./nachos-final -cp step5_seek run

# Test case
OUTPUT=$(./nachos-final -x run | grep "Victory")

echo "$OUTPUT"

#Clean up test
./nachos-final -f

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
