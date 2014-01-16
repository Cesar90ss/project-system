NACHOS_DIR=../build/

cd $NACHOS_DIR
OUTPUT=$(./nachos-step4 -x test_return | grep "B\n")

if [ -n "$OUTPUT" ]; then
    exit -1
    else
        exit 0
	fi

