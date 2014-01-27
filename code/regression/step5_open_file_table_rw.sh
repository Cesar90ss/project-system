NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f

echo 1 > /tmp/x.txt
./nachos-step5 -cp /tmp/x.txt 1
echo 2 > /tmp/x.txt
./nachos-step5 -cp /tmp/x.txt 2
echo 3 > /tmp/x.txt
./nachos-step5 -cp /tmp/x.txt 3
echo 4 > /tmp/x.txt
./nachos-step5 -cp /tmp/x.txt 4
echo 5 > /tmp/x.txt
./nachos-step5 -cp /tmp/x.txt 5
echo 6 > /tmp/x.txt
./nachos-step5 -cp /tmp/x.txt 6


./nachos-step5 -cp open_file_table_rw run

# Test case

OUTPUT=$(./nachos-step5 -x run -rs 4 | grep "123456789123")

if [ -n "$OUTPUT" ]; then
    RETURN=0
else
    echo "$OUTPUT"
    RETURN=-1
fi

#Clean up test
./nachos-step5 -f

#return test result
exit $RETURN
