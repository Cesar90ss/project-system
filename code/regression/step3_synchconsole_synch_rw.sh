NACHOS_DIR=../build/

cd $NACHOS_DIR

# Based on line number, should be == 1010
./nachos-final -cp synchconsole_concurrent_rw run
OUTPUT=$(echo "s" | ./nachos-final -x run -rs 2| wc -l)
if [ $OUTPUT -eq 1010 ]; then
    exit 0
else
    exit -1
fi
