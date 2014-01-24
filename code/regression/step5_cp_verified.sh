NACHOS_DIR=../build/

cd $NACHOS_DIR

#format disk
./nachos-step5 -f 

#copy a file
./nachos-step5 -cp ../Makefile Makefile

#save the print output in a temporary file
./nachos-step5 -p Makefile | head -n 10 | head -n +4 > step5_cp_verified.temp

OUT1=$(cat ../Makefile | head -n +4)

# Test case

if [ -z "$(diff $OUT1 step5_cp_verified.temp)" ]; then
    RETURN=0
else
    RETURN=-1
fi

#Clean up test
./nachos-step5 -f

#delete temporary file
#rm step5_cp_verified.temp

#return test result
echo $RETURN

