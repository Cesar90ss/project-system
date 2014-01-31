NACHOS_DIR=../../../code/build/
cd $NACHOS_DIR

#format disk
./nachos-final -f

#create file structure
echo "Clean up system file"
./nachos-final -f
read -p "Press [Enter] to continue..."

echo "Creating directory test"
./nachos-final -md test
./nachos-final -lr /
read -p "Press [Enter] to continue..."

echo "Creating directory test in test2"
./nachos-final -md /test/test2
./nachos-final -lr /

echo "Creating directory test in test2"
./nachos-final -md /test/test2/test3
./nachos-final -lr /

echo "Trying to create a directory with invalid name"
./nachos-final -md "//d/sq/"

echo "Copying Makefile in /test/test2"
./nachos-final -cp ../Makefile Makefile
./nachos-final -lr
