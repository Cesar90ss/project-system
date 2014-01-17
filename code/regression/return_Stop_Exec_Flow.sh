NACHOS_DIR=../build/

cd $NACHOS_DIR
OUTPUT=$(./nachos-step4 -x test_return | grep -E "AAA[^BBB]")

if [ -n "$OUTPUT" ]; then
    exit 0
else
    exit -1
fi
