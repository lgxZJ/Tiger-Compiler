OBJECTS := test_myError.o myError.o makeMemory.o testHelper.o

COMPILE_FLAGS := gcc -g -c -std=c99
ADDITIONAL_LIB := -L/home/lgxZJ/Libraries/CUnit/lib
ADDITIONAL_INCLUDE := -I/home/lgxZJ/Libraries/CUnit/include
COVERAGE_OPTIONS := -fprofile-arcs -ftest-coverage

VPATH := ../../:../

all.out : $(OBJECTS)
	gcc $(ADDITIONAL_LIB) $(ADDITIONAL_INCLUDE) -lcunit -coverage -o all.out $(OBJECTS)

%.o : %.c
	$(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) $<

myError.o : myError.c
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
	genhtml -f coverage.info
	firefox index.html
