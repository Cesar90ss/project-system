#!/bin/bash

VALGRIND_LOG=valgrind.log

# Make clean from beginning
echo "Build"
make -j >/dev/null

# Move step4 & step5 to override
echo "Move"
if [[ ! "$(file build/nachos-final)" =~ "shell" ]]; then
    mv build/{,_}nachos-final
else
    echo "Warning: seems to be already installed"
fi

# Clean previous log
echo "Clean"
rm -f build/$VALGRIND_LOG

# Set up hook script
echo "Hook final"
echo "#!/bin/bash
echo '=======================================' >> $VALGRIND_LOG
echo ./nachos-final \$@ >> $VALGRIND_LOG
echo '=======================================' >> $VALGRIND_LOG
if [ -n \"\$(echo \$@ | grep \"\\-x\")\" ]; then # Remove this if you want to
                                                # check for all things (very
                                                # long...)
    valgrind --track-origins=yes --leak-check=full --show-reachable=yes ./_nachos-final \$@ 2>>$VALGRIND_LOG
else
    ./_nachos-final \$@
fi
echo '=======================================' >> $VALGRIND_LOG
echo >> $VALGRIND_LOG
" > build/nachos-final
chmod +x build/nachos-final

