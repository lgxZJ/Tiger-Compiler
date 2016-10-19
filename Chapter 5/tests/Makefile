#	to re-test all tests when one changed

MAKE := /bin/make

retest-all:
	cd myEnvironment && $(MAKE) retest
	cd ..
	cd myError && $(MAKE) retest
	cd ..
	cd mySemantic && $(MAKE) retest
	cd ..
	cd myTable && $(MAKE) retest
	cd ..
	cd typeChecker && $(MAKE) retest
	cd ..
	cd typeMaker && $(MAKE) retest
	cd ..
	cd recursiveDecs && $(MAKE) retest
	cd ..
	cd mySymbol && $(MAKE) retest
	cd ..