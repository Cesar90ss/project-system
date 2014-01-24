NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f

#create file structure
./nachos-step5 -cp ../Makefile a

# Test case

# Make directory
./nachos-step5 -md dir1
./nachos-step5 -md dir2
./nachos-step5 -md dir3
./nachos-step5 -md dir4
./nachos-step5 -md dir5
./nachos-step5 -md dir6
./nachos-step5 -md dir7
./nachos-step5 -md dir8
./nachos-step5 -md dir9
./nachos-step5 -rd dir5
./nachos-step5 -rd dir6
./nachos-step5 -md dir10
./nachos-step5 -md dir11
./nachos-step5 -md dir12

OUTPUT=$(./nachos-step5 -l / | head -n -10 | tr "\n" "|")
EXPECTED_OUTPUT="f - a|d - dir1|d - dir2|d - dir3|d - dir4|d - dir7|d - dir8|d - dir9|d - dir10|d - dir11|"

if [ "$OUTPUT" = "$EXPECTED_OUTPUT" ]; then
    RETURN=0
else
    echo "$OUTPUT"
    RETURN=-1
fi

#Clean up test
./nachos-step5 -f

#return test result
exit $RETURN
