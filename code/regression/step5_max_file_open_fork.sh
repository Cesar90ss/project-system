NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f

./nachos-final -md a
./nachos-final -cp ../Makefile a/0
./nachos-final -cp ../Makefile a/1
./nachos-final -cp ../Makefile a/2
./nachos-final -cp ../Makefile a/3
./nachos-final -cp ../Makefile a/4
./nachos-final -cp ../Makefile a/5
./nachos-final -cp ../Makefile a/6
./nachos-final -cp ../Makefile a/7
./nachos-final -cp ../Makefile a/8
./nachos-final -cp ../Makefile a/9

./nachos-final -cp max_file_open_fork run
./nachos-final -cp max_file_open run2

# Test case
./nachos-final -ls
./nachos-final -x run -rs 4
OUTPUT=$(./nachos-final -x run -rs 4| grep  "okok")

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
