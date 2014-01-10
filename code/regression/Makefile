# Target for on test program
ALL_TESTS=$(wildcard *.sh)
ERROR_LOG=error.log

MAKE=make
BASH=bash

# Static targets for non re-generatation
.PHONY: create_test regression $(ALL_TESTS) clean

# First rules is running all tests
first: regression

# Clean test error log
clean:
	echo > $(ERROR_LOG)

# Launch all test
regression: clean $(ALL_TESTS)

# Check for a test
$(ALL_TESTS):
	@echo -n "[Test] Running $* : "

	@TMPNAME=`mktemp`; \
	$(BASH) ./$@ >$$TMPNAME 2>&1; \
	EXIT_CODE=$$?; \
	if [ $$EXIT_CODE -ne 0 ]; then \
		echo -e "\033[0;31merror\033[m"; \
		echo -n "[Test] Description : "; \
		cat $*.desc; \
		echo "[Test] Exit code : $$EXIT_CODE"; \
		echo "==== Error for $@ code $$EXIT_CODE ====" >> $(ERROR_LOG); \
		cat $$TMPNAME >> $(ERROR_LOG); \
		echo >> $(ERROR_LOG); \
		echo "See $(ERROR_LOG) to get output of $@"; \
		rm $$TMPNAME; \
	else \
		echo -e "\033[0;32mok\033[m"; \
		rm $$TMPNAME; \
	fi;

# Create a test
create_test:
	@echo -n "Shell script name (without .sh): "; \
	read TEST_NAME; \
	if [ ! -f $$TEST_NAME.sh ]; then \
		echo "No file name $$TEST_NAME.sh"; \
		exit 0; \
	fi; \
	echo "Test Description : (stop with CTRL+D)"; \
	cat >$$TEST_NAME.desc; \
	echo -e "\nChecking test...\n"; \
	${MAKE} $$TEST_NAME.sh



