OBJECTS := test_typeMaker.o typeMaker.o makeMemory.o\
	mySymbol.o myTable.o testHelper.o
COMPILE_FLAG := gcc -g -c -std=c99

VPATH := ../../:../

ANOTHER_INCLUDE := -I/home/lgxZJ/Libraries/CUnit/include
ANOTHER_LIB := -L/home/lgxZJ/Libraries/CUnit/lib 

tests.out : $(OBJECTS)
	gcc -g $(ANOTHER_LIB) -lcunit -coverage -o tests.out $(OBJECTS)

typeMaker.o : typeMaker.c
	$(COMPILE_FLAG) $(ANOTHER_INCLUDE) -fprofile-arcs -ftest-coverage $<

%.o : %.c
	$(COMPILE_FLAG) $(ANOTHER_INCLUDE) $<

test_typeMaker.o : test_typeMaker.c typeMaker.h
	$(COMPILE_FLAG)  $(ANOTHER_INCLUDE) $^

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
