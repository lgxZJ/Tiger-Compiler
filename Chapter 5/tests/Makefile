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
	cd breakChecker && $(MAKE) retest
	cd ..

coverage-all:
	cd myEnvironment && $(MAKE) show-coverage
	cd ..
	cd myError && $(MAKE) show-coverage
	cd ..
	cd mySemantic && $(MAKE) show-coverage
	cd ..
	cd myTable && $(MAKE) show-coverage
	cd ..
	cd typeChecker && $(MAKE) show-coverage
	cd ..
	cd typeMaker && $(MAKE) show-coverage
	cd ..
	cd recursiveDecs && $(MAKE) show-coverage
	cd ..
	cd mySymbol && $(MAKE) show-coverage
	cd ..
	cd breakChecker && $(MAKE) show-coverage
	cd ..
