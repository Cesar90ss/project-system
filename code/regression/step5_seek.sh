NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f

echo "abcd" > /tmp/test
#copy c file in nachos
./nachos-step5 -cp /tmp/test test
./nachos-step5 -cp step5_seek run

# Test case
OUTPUT=$(./nachos-step5 -x run | grep "Victory")

echo "$OUTPUT"

#Clean up test
./nachos-step5 -f

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
