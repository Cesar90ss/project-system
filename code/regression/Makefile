# Target for on test program
ALL_TESTS=$(wildcard *.sh)
ERROR_LOG=error.log

MAKE=make
BASH=bash
ECHO=/bin/echo # Using echo binary instead of bash builtin (problem with -e)

# Static targets for non re-generatation
.PHONY: create_test regression $(ALL_TESTS) clean

# First rules is running all tests
first: regression

# Clean test error log
clean:
	@echo > $(ERROR_LOG)

# Launch all test
regression: clean $(ALL_TESTS)

# Check for a test
$(ALL_TESTS):
	@echo -n "[Test] Running $* : "

	@TMPNAME=`mktemp`; \
	$(BASH) ./$@ >$$TMPNAME 2>&1; \
	EXIT_CODE=$$?; \
	if [ $$EXIT_CODE -ne 0 ]; then \
		${ECHO} -e "\033[0;35merror\033[m"; \
		${ECHO} -n "[Test] Description : "; \
		cat $*.desc; \
		${ECHO} "[Test] Exit code : $$EXIT_CODE"; \
		${ECHO} "==== Error for $@ code $$EXIT_CODE ====" >> $(ERROR_LOG); \
		cat $$TMPNAME >> $(ERROR_LOG); \
		${ECHO} >> $(ERROR_LOG); \
		${ECHO} "See $(ERROR_LOG) to get output of $@"; \
		rm $$TMPNAME; \
	else \
		${ECHO} -e "\033[0;32mok\033[m"; \
		rm $$TMPNAME; \
	fi;

# Create a test
create_test:
	@${ECHO} -n "Shell script name (without .sh): "; \
	read TEST_NAME; \
	if [ ! -f $$TEST_NAME.sh ]; then \
		${ECHO} "No file name $$TEST_NAME.sh"; \
		exit 0; \
	fi; \
	${ECHO} "Test Description : (stop with CTRL+D)"; \
	cat >$$TEST_NAME.desc; \
	${ECHO} -e "\nChecking test...\n"; \
	${MAKE} $$TEST_NAME.sh



