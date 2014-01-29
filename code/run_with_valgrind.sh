#!/bin/bash

VALGRIND_LOG=valgrind.log

# Make clean from beginning
echo "Build"
make -j >/dev/null

# Move step4 & step5 to override
echo "Move"
mv build/{,_}nachos-final

# Clean previous log
echo "Clean"
rm -f build/$VALGRIND_LOG

# Set up hook script
echo "Hook final"
echo "#!/bin/bash
echo '=======================================' >> $VALGRIND_LOG
echo ./nachos-final \$@ >> $VALGRIND_LOG
echo '=======================================' >> $VALGRIND_LOG
valgrind --leak-check=full --show-reachable=yes ./_nachos-final \$@ 2>>$VALGRIND_LOG
echo '=======================================' >> $VALGRIND_LOG
echo >> $VALGRIND_LOG
" > build/nachos-final
chmod +x build/nachos-final

