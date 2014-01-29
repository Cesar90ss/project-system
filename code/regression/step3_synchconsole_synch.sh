NACHOS_DIR=../build/

cd $NACHOS_DIR

# Only two differents lines (no mixed up message)
./nachos-final -cp synchconsole_concurrent run
OUTPUT=$(./nachos-final -x run -rs 4 | head -n -10 | sort -u | wc -l)
if [ $OUTPUT -eq 2 ]; then
    exit 0
else
    exit -1
fi
