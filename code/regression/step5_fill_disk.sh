NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f

#create 200K file in /tmp
rm -f /tmp/test
dd if=/dev/zero of=/tmp/test bs=120K count=1

#create file structure
./nachos-step5 -cp /tmp/test test1
./nachos-step5 -cp /tmp/test test2
./nachos-step5 -cp ../Makefile test3

# Test case
./nachos-step5 -l /
OUTPUT=$(./nachos-step5 -l / | head -n -10 | tr "\n" "|")
EXPECTED_OUTPUT="f - test1|f - test3|"

if [ "$OUTPUT" = "$EXPECTED_OUTPUT" ]; then
    RETURN=0
else
    RETURN=-1
fi

#Clean up test
./nachos-step5 -f
rm -f /tmp/test

#return test result
exit $RETURN
