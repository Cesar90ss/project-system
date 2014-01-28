NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f

./nachos-step5 -cp step5_threads_write run

OUTPUT=$(./nachos-step5 -x run -rs 4 | sed 's/./&\n/300' | sort | tr -d "\n")

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
echo "$OUTPUT"
echo "$EXPECTED_OUTPUT"

if [ "$OUTPUT"="$EXPECTED_OUTPUT" ]; then
    exit 0
else
    exit -1
fi
