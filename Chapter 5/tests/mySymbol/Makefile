OBJECTS := test_mySymbol.o makeMemory.o mySymbol.o myTable.o testHelper.o

COMPILE_FLAGS := gcc -g -c -std=c99
ADDITIONAL_INCLUDE := -I/home/lgxZJ/Libraries/CUnit/include
ADDITIONAL_LIB := -L/home/lgxZJ/Libraries/CUnit/lib
COVERAGE_OPTIONS := -fprofile-arcs -ftest-coverage

VPATH := ../../:../

all.out : $(OBJECTS)
	gcc $(ADDITIONAL_INCLUDE) $(ADDITIONAL_LIB) -lcunit -coverage -o all.out $(OBJECTS)

mySymbol.o : mySymbol.c
	$(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) $(COVERAGE_OPTIONS) $<

%.o : %.c
	$(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) $<



.PHONY: clean run retest show-coverage

clean:
	rm -f $(OBJECTS) all.out *.gcda *.gcno

run:
	./all.out

retest:
	make clean
	make
	./all.out

show-coverage:
	lcov --directory . --zerocounters
	./all.out
	lcov --directory . --capture --output-file code-coverage.info
	genhtml code-coverage.info
	firefox index.html
