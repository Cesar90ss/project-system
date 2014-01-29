NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-final -f

#create file structure
rm /tmp/test
for i in {1..300}; do
    echo -n "A" >> /tmp/test
done
for i in {1..300}; do
    echo -n "B" >> /tmp/test
done
for i in {1..300}; do
    echo -n "C" >> /tmp/test
done
for i in {1..300}; do
    echo -n "D" >> /tmp/test
done
for i in {1..300}; do
    echo -n "E" >> /tmp/test
done
for i in {1..300}; do
    echo -n "F" >> /tmp/test
done
for i in {1..300}; do
    echo -n "I" >> /tmp/test
done


./nachos-final -cp /tmp/test test

./nachos-final -cp rw_concurent_read run
./nachos-final -x run -rs 4
OUTPUT=$(./nachos-final -x run -rs 4 | grep "END")

#Clean up test
./nachos-final -f

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
