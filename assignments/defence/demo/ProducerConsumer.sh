NACHOS_DIR=../../../code/build/

cd $NACHOS_DIR
./nachos-final -f > /dev/null
./nachos-final -cp threadProdConsDemo run > /dev/null
./nachos-final -cp step6_test_simple_listen_connect run2  > /dev/null


OUTPUT=$(./nachos-final -m 0 -rs 2 -x run  | head -n -10)

echo "$OUTPUT"
