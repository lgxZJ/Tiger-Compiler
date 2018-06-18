all:
	$(MAKE)  -C ./Compiler

run-tests:
	$(MAKE) -C ./Compiler/tests
	cd ./Compiler
	coveralls -r . -b tests/myTemp/
	cd ..
