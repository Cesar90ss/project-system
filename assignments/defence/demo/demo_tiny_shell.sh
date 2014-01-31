./nachos-final -f
./nachos-final -cp getstringeof run3
./nachos-final -cp step4_tiny_shell_test_program2 run1
./nachos-final -cp step4_tiny_shell_test_program1 run2
./nachos-final -cp step4_tiny_shell run

printf "run1 &\nrun2\nfg 0\nrun3\n" | ./nachos-final -x run -rs 2