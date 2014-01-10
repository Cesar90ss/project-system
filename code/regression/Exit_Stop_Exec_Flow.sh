NACHOS_DIR=../build/

cd $NACHOS_DIR
OUTPUT=$(./nachos-step2 -x test_exit | grep "B\n")

if [ -n "$OUTPUT" ]; then
    exit -1
    else
        exit 0
	fi

