NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f

#create file structure
echo -n "0123456789" > /tmp/test1

./nachos-step5 -cp /tmp/test1 test
./nachos-step5 -cp step5_thread_close_read run
./nachos-step5 -x run -rs 4
OUTPUT=$(./nachos-step5 -x run -rs 4 | grep "END")

#Clean up test
./nachos-step5 -f

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
