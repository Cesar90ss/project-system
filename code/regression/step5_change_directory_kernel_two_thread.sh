NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f

#create file structure
echo -n "test1" > /tmp/test1
echo -n "test2" > /tmp/test2

./nachos-final -cp /tmp/test1 test
./nachos-final -md a
./nachos-final -cp /tmp/test2 a/test

# Test case

./nachos-final -tk2

if [ $? -eq 0 ]; then
    RETURN=0
else
    echo "Exit code $?"
    RETURN=-1
fi

#Clean up test
./nachos-final -f

#return test result
exit $RETURN
