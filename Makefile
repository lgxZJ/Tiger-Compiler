all:
	$(MAKE)  -C ./Compiler

clean-tests:
	$(MAKE) -C ./Compiler/tests clean-tests
make-tests:
	$(MAKE) -C ./Compiler/tests make-tests
run-tests:
	export LD_LIBRARY_PATH=/usr/local/lib; $(MAKE) -C ./Compiler/tests run-tests
