NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f 

#copy a file
./nachos-final -cp ../Makefile Makefile

#save the print output in a temporary file
./nachos-final -p Makefile | head -n 10 | head -n +4 > temp1

cat ../Makefile | head -n +4 > temp2

#Clean up test
./nachos-final -f

#delete temporary file

# Test case
if [ -z "$(diff temp1 temp2)" ]; then
    rm temp1
    rm temp2
    exit 0
else
    exit -1
fi
