 NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f

./nachos-final -md a
./nachos-final -md b
./nachos-final -cp ../Makefile a/0
./nachos-final -cp ../Makefile a/1
./nachos-final -cp ../Makefile b/1
./nachos-final -cp ../Makefile a/2
./nachos-final -cp ../Makefile b/2
./nachos-final -cp ../Makefile a/3
./nachos-final -cp ../Makefile a/4
./nachos-final -cp ../Makefile a/5
./nachos-final -cp ../Makefile a/6
./nachos-final -cp ../Makefile a/7
./nachos-final -cp ../Makefile a/8
./nachos-final -cp ../Makefile a/9

./nachos-final -cp max_file_open_thread run

# Test case
./nachos-final -ls
OUTPUT=$(./nachos-final -x run -rs 4| grep  "ok")

if [ -n "$OUTPUT" ]; then
    RETURN=0
else
    echo "$OUTPUT"
    RETURN=-1
fi

#Clean up test
./nachos-final -f

#return test result
exit $RETURN
