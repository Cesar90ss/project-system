NACHOS_DIR=../build/

cd $NACHOS_DIR
./nachos-final -cp test_exit run
OUTPUT=$(./nachos-final -x run | grep -E "AAA[^BBB]")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
