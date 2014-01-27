NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f

./nachos-step5 -cp remove_file run

# Test case
OUTPUT=$(./nachos-step5 -x run && ./nachos-step5 -l /| tr "\n" "|" | grep "1f - run|")

#Clean up test
./nachos-step5 -f

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
