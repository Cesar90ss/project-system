NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f 

#create a 30kb file in /tmp
dd if=/dev/zero of=/tmp/test bs=1024 count=30

#create file structure
./nachos-step5 -cp /tmp/test test

# Test case
OUTPUT=$(./nachos-step5 -l / | head -n -10 | tr "\n" "|")
EXPECTED_OUTPUT="f - test|"

if [ "$OUTPUT" = "$EXPECTED_OUTPUT" ]; then
    RETURN=0
else
    RETURN=-1
fi

#Clean up test
./nachos-step5 -f

#return test result
exit $RETURN
