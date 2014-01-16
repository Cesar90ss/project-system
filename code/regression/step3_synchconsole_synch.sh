NACHOS_DIR=../build/

cd $NACHOS_DIR

# Only two differents lines (no mixed up message)
OUTPUT=$(./nachos-step4 -x synchconsole_concurrent -rs 2 | head -n -10 | sort -u | wc -l)
if [ $OUTPUT -eq 2 ]; then
    exit 0
else
    exit -1
fi
