#	to re-test all tests when one changed

MAKE := /bin/make

retest-all:
	cd myEnvironment && $(MAKE) retest
	cd ..
	cd myTemp && $(MAKE) retest
	cd ..
	cd mySemantic && $(MAKE) retest
	cd ..
	cd myFrame && $(MAKE) retest
	cd ..
	cd recursiveDecs && $(MAKE) retest
	cd ..
	cd myTranslate && $(MAKE) retest
	cd ..
	cd forChecker && $(MAKE) retest
	cd ..
	cd myEscape && $(MAKE) retest
	cd ..
	cd lexString && $(MAKE) retest
	cd ..
	cd integrationTests && $(MAKE) retest
	cd ..

coverage-all:
	cd myEnvironment && $(MAKE) show-coverage
	cd ..
	cd myTemp && $(MAKE) show-coverage
	cd ..
	cd mySemantic && $(MAKE) show-coverage
	cd ..
	cd myFrame && $(MAKE) show-coverage
	cd ..
	cd recursiveDecs && $(MAKE) show-coverage
	cd ..
	cd myTranslate && $(MKAE) show-coverage
	cd ..
	cd forChecker && $(MAKE) show-coverage
	cd ..
	cd myEscape && $(MAKE) show-coverage
	cd ..
	cd myEscape && $(MAKE) show-coverage
	cd ..
