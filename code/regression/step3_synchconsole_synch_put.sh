NACHOS_DIR=../build/

cd $NACHOS_DIR

# Only two differents lines (no mixed up message)
SA=$(for i in {1..10}; do printf "%s" "a"; done;)
SB=$(for i in {1..10}; do printf "%s" "b"; done;)
S=
for i in {1..100}; do
    S=$S$SA$SB
done


./nachos-final -cp synchconsole_concurrent_put run
OUTPUT=$(echo -n $S | ./nachos-final -x run -rs 2 | head -n -10 | sort -u | wc -l)
if [ $OUTPUT -eq 2 ]; then
    exit 0
else
    exit -1
fi
