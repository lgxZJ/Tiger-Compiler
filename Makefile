all:
	$(MAKE)  -C ./Compiler

run-tests:
	$(MAKE) -C ./Compiler/tests
