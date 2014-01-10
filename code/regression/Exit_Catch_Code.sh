NACHOS_DIR=../build/

cd $NACHOS_DIR
OUTPUT=$(./nachos-step2 -x test_exit_1 -d a| grep "Exit program, return code exit(1)\n")

if [ -n "$OUTPUT" ]; then
    exit -1
    else
        exit 0
	fi

