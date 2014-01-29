NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f

#create a 30kb file in /tmp
dd if=/dev/zero of=/tmp/test bs=30K count=1

#create file structure
./nachos-final -cp /tmp/test test

# Test case
OUTPUT=$(./nachos-final -l / | head -n -10 | tr "\n" "|")
EXPECTED_OUTPUT="f - test|"

if [ "$OUTPUT" = "$EXPECTED_OUTPUT" ]; then
    RETURN=0
else
    RETURN=-1
fi

#Clean up test
./nachos-final -f
rm /tmp/test

#return test result
exit $RETURN
