OBJECTS := test_myEnvironment.o myEnvironment.o mySymbol.o makeMemory.o \
	typeMaker.o myTable.o testHelper.o

ADDITION_LIB := -L/home/lgxZJ/Libraries/CUnit/lib
ADDITION_INCLUDE := -I/home/lgxZJ/Libraries/CUnit/include
COMPILE_FLAGS = gcc -g -c -std=c99

VPATH := ../../:../


tests.out: $(OBJECTS)
	gcc -g $(ADDITION_LIB) -lcunit -coverage -o tests.out $(OBJECTS)

myEnvironment.o : myEnvironment.c
	$(COMPILE_FLAGS) $(ADDITION_INCLUDE) -fprofile-arcs -ftest-coverage $<

%.o : %.c
	$(COMPILE_FLAGS) $(ADDITION_INCLUDE) $<


.PHONY: clean run retest show-coverage
clean:
	rm -f $(OBJECTS) tests.out

run:
	./tests.out

retest:
	make clean
	make
	./tests.out

show-coverage:
	lcov --directory . --zerocounters
	./tests.out
	lcov --directory . --capture --output-file coverage.info
	genhtml coverage.info
	firefox index.html
