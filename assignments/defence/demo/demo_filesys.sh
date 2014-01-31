NACHOS_DIR=../../../code/build/
cd $NACHOS_DIR

function r () 
{
    echo $@
    $@ | head -n -10
    ./nachos-final -lr / | head -n -10
}

#create file structure
echo "Clean up system file"
r ./nachos-final -f
read -p "Press [Enter] to continue..."

echo -e "\nCreating directory test"
r ./nachos-final -md test 
read

echo -e "\nCreating directory test2 in test"
r ./nachos-final -md /test/test2
read

echo -e "\nCreating directory test3 in /test/test2"
r ./nachos-final -md /test/test2/test3
read

echo -e "\nCreating directory removeMe in root"
r ./nachos-final -md /removeMe
read

echo -e "\nTrying to create a directory with invalid name"
r ./nachos-final -md "d///sq/"
read

echo -e "\nCopying Makefile in /test/test2"
r ./nachos-final -cp ../Makefile /test/test2/Makefile
read

echo -e "\nRemoving removeMe directory"
r ./nachos-final -rd /removeMe
read


echo -e "\nCreating a file big file of 200KO"
dd if=/dev/zero of=/tmp/bigfile bs=200K count=1

echo -e "\nCopying "
r ./nachos-final -cp /tmp/bigfile /imp
read

r ./nachos-final -cp TheEnd run
./nachos-final -x run
