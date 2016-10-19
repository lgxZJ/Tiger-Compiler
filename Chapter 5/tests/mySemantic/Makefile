OBJECTS := test_mySemantic.o mySemantic.o myEnvironment.o\
	mySymbol.o typeMaker.o makeMemory.o myTable.o abstractSyntaxMaker.o\
	myError.o typeChecker.o testHelper.o recursiveDecs.o

COMPILE_FLAGS := gcc -g -c -std=c99
ADDITIONAL_LIB := -L/home/lgxZJ/Libraries/CUnit/lib
ADDITIONAL_INCLUDE := -I/home/lgxZJ/Libraries/CUnit/include

VPATH := ../../:../

all : $(OBJECTS)
	gcc -g $(ADDITIONAL_LIB) -lcunit -coverage -o  tests.out $(OBJECTS)

# make rules
%.o : %.c
	$(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) $<
mySemantic.o : mySemantic.c
	$(COMPILE_FLAGS) -fprofile-arcs -ftest-coverage $(ADDITIONAL_INCLUDE) $<

.PHONY: clean run show-coverage rebuild
clean:
	rm $(OBJECTS) tests.out *.gcda *.gcno

run:
	./tests.out

show-coverage:
	lcov --directory . --zerocounters
	./tests.out
	lcov --directory . --capture --output-file code-coverage.info
	genhtml code-coverage.info
	firefox index.html

retest:
	make clean
	make
	./tests.out
