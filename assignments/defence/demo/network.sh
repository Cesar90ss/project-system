NACHOS_DIR=../../../code/build/

cd $NACHOS_DIR
./nachos-final -f > /dev/null
./nachos-final -cp step6_test_simple_listen_accept run  > /dev/null
./nachos-final -cp step6_test_simple_listen_connect run2  > /dev/null


./nachos-final -m 0 -rs 2 -x run  | head -n -10 > out1 &
./nachos-final -m 1 -rs 4 -x run2  | head -n -10 > out2

echo PROGRAM 1
echo "$(cat out1)"
echo PROGRAM 2
echo "$(cat out2)"

rm out1
rm out2
