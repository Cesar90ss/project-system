NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f

./nachos-final -cp remove_file run

# Test case
OUTPUT=$(./nachos-final -x run && ./nachos-final -l /| tr "\n" "|" | grep "1f - run|")

#Clean up test
./nachos-final -f

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
