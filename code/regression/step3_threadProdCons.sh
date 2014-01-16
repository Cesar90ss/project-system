#!/bin/bash
NACHOS_DIR=../build/

cd $NACHOS_DIR
OUTPUT=`./nachos-step4 -rs 4 -x threadProdCons | head -n -10`
M=$(echo "$OUTPUT" | sort | uniq -c | uniq -w 1)
if [ $( echo $M | wc -l ) -ne "1" ]; then
	exit -1
fi


M=$(echo $M | head -c 1)

if [ $M -eq "2" ]; then
	exit 0
else
	exit -2
fi
