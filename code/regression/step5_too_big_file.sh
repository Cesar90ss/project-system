NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f

#create 200K file in /tmp
rm /tmp/test
dd if=/dev/zero of=/tmp/test bs=200K count=1
./nachos-final -md a

#create file structure
./nachos-final -cp /tmp/test test

# Test case
OUTPUT=$(./nachos-final -l / | head -n -10 | tr "\n" "|")
EXPECTED_OUTPUT="d - a|"

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
