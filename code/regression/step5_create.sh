NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f

#copy c file in nachos
./nachos-step5 -cp step5_create run

./nachos-step5 -x run
# Test case
OUTPUT=$(./nachos-step5 -ls | grep "test")

#Clean up test
./nachos-step5 -f

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
