NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f

echo -n "" > /tmp/test
./nachos-step5 -cp /tmp/test test
./nachos-step5 -cp step5_threads_write run

./nachos-step5 -x run -rs 4

OUTPUT=$(./nachos-step5 -p test | sed 's/./&\n/300' | sort | tr -d "\n")

for i in {1..300}; do
    echo -n "a" >> /tmp/test
done
for i in {1..300}; do
    echo -n "b" >> /tmp/test
done
for i in {1..300}; do
    echo -n "c" >> /tmp/test
done
for i in {1..300}; do
    echo -n "d" >> /tmp/test
done
for i in {1..300}; do
    echo -n "e" >> /tmp/test
done
for i in {1..300}; do
    echo -n "f" >> /tmp/test
done
for i in {1..300}; do
    echo -n "i" >> /tmp/test
done
EXPECTED_OUTPUT=$(cat /tmp/test)

#Clean up test
./nachos-step5 -f

if [ "$OUTPUT"="$EXPECTED_OUTPUT" ]; then
    exit 0
else
    exit -1
fi
