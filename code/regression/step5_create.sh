NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f

#copy c file in nachos
./nachos-final -cp step5_create run

./nachos-final -x run
# Test case
OUTPUT=$(./nachos-final -ls | grep "test")

#Clean up test
./nachos-final -f

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
