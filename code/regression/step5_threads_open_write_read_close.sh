NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f

#create file structure
echo -n "1" > /tmp/test1

./nachos-final -cp /tmp/test1 test
./nachos-final -cp step5_threads_open_write_read_close run

OUTPUT=$(./nachos-final -x run -rs 4| grep "OK")

#Clean up test
./nachos-final -f

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
