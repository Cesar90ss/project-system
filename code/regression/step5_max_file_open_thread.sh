 NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f

./nachos-step5 -md a
./nachos-step5 -md b
./nachos-step5 -cp ../Makefile a/0
./nachos-step5 -cp ../Makefile a/1
./nachos-step5 -cp ../Makefile b/1
./nachos-step5 -cp ../Makefile a/2
./nachos-step5 -cp ../Makefile b/2
./nachos-step5 -cp ../Makefile a/3
./nachos-step5 -cp ../Makefile a/4
./nachos-step5 -cp ../Makefile a/5
./nachos-step5 -cp ../Makefile a/6
./nachos-step5 -cp ../Makefile a/7
./nachos-step5 -cp ../Makefile a/8
./nachos-step5 -cp ../Makefile a/9

./nachos-step5 -cp max_file_open_thread run

# Test case
./nachos-step5 -ls
OUTPUT=$(./nachos-step5 -x run -rs 4| grep  "ok")

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
