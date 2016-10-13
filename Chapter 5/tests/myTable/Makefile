OBJECTS := test_myTable.o testHelper.o makeMemory.o myTable.o

COMPILE_FLAGS := gcc -g -c -std=c99
COVERAGE_OPTIONS := -fprofile-arcs -ftest-coverage
ADDITIONAL_INCLUDE := -I/home/lgxZJ/Libraries/CUnit/include
ADDITIONAL_LIB := -L/home/lgxZJ/Libraries/CUnit/lib

VPATH := ../../:../

all.out : $(OBJECTS)
	gcc -g $(ADDITIONAL_INCLUDE) $(ADDITIONAL_LIB) -lcunit -coverage -o all.out $(OBJECTS)

%.o : %.c
	$(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) $<

myTable.o : myTable.c
	$(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) $(COVERAGE_OPTIONS) $<



.PHONY : clean retest run show-coverage
clean:
	rm -f $(OBJECTS) all.out

retest:
	make clean
	make
	./all.out

run:
	./all.out

show-coverage:
	lcov --directory . --zerocounters
	./all.out
	lcov --directory . --capture --output-file coverage.info
	genhtml coverage.info
	firefox index.html


