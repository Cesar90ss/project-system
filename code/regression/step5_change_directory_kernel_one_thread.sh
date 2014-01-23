NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f

#create file structure
echo -n "test1" > /tmp/test1
echo -n "test2" > /tmp/test2

./nachos-step5 -cp /tmp/test1 test
./nachos-step5 -md a
./nachos-step5 -cp /tmp/test2 a/test

# Test case

./nachos-step5 -tk1

if [ $? -eq 0 ]; then
    RETURN=0
else
    echo "Exit code $?"
    RETURN=-1
fi

#Clean up test
./nachos-step5 -f

#return test result
exit $RETURN
