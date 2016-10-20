OBJECTS := test_breakChecker.o breakChecker.o testHelper.o

COMPILE_FLAGS := gcc -g -c -std=c99
ADDITIONAL_INCLUDE := -I/home/lgxZJ/Libraries/CUnit/include
ADDITIONAL_LIB := -L/home/lgxZJ/Libraries/CUnit/lib
COVERAGE_OPTIONS := -fprofile-arcs -ftest-coverage

VPATH := ../../:../


all.out : $(OBJECTS)
	gcc -g $(ADDITIONAL_LIB) -lcunit -coverage -o all.out $(OBJECTS)

breakChecker.o : breakChecker.c
	$(COMPILE_FLAGS)  $(ADDITIONAL_INCLUDE) $(COVERAGE_OPTIONS) $<

%.o : %.c
	$(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) $<



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
