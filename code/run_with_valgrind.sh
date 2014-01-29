#!/bin/bash

VALGRIND_LOG=valgrind.log

# Make clean from beginning
echo "Build"
make -j >/dev/null

# Move step4 & step5 to override
echo "Move"
mv build/{,_}nachos-step4
mv build/{,_}nachos-step5

# Clean previous log
echo "Clean"
rm -f build/$VALGRIND_LOG

# Set up hook script
echo "Hook step4"
echo "#!/bin/bash
echo '=======================================' >> $VALGRIND_LOG
echo ./nachos-step4 \$@ >> $VALGRIND_LOG
echo '=======================================' >> $VALGRIND_LOG
valgrind --leak-check=full --show-reachable=yes ./_nachos-step4 \$@ 2>>$VALGRIND_LOG
echo '=======================================' >> $VALGRIND_LOG
echo >> $VALGRIND_LOG
" > build/nachos-step4
chmod +x build/nachos-step4

echo "Hook step5"
echo "#!/bin/bash
echo '=======================================' >> $VALGRIND_LOG
echo ./nachos-step5 \$@ >> $VALGRIND_LOG
echo '=======================================' >> $VALGRIND_LOG
valgrind --leak-check=full --show-reachable=yes ./_nachos-step5 \$@ 2>>$VALGRIND_LOG
echo '=======================================' >> $VALGRIND_LOG
echo >> $VALGRIND_LOG
" > build/nachos-step5
chmod +x build/nachos-step5

