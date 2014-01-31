#!/bin/bash
NACHOS_DIR=../../../code/build/

cd $NACHOS_DIR
pwd
./nachos-final -f > /dev/null
./nachos-final -cp getstringeof run3 > /dev/null
./nachos-final -cp step4_tiny_shell_test_program2 run1 > /dev/null
./nachos-final -cp step4_tiny_shell_test_program1 run2 > /dev/null
./nachos-final -cp step4_tiny_shell run > /dev/null

./nachos-final -x run -rs 2
