NACHOS_DIR=../build/

cd $NACHOS_DIR

# Based on line number, should be == 1010
OUTPUT=$(echo "s" | ./nachos-step4 -x synchconsole_concurrent_rw -rs 2 | wc -l)
if [ $OUTPUT -eq 1010 ]; then
    exit 0
else
    exit -1
fi
