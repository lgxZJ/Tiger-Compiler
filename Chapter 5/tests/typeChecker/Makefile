OBJECTS := test_typeChecker.o typeChecker.o typeMaker.o makeMemory.o\
	mySymbol.o testHelper.o myTable.o

VPATH := ../../:../

COMPILE_FLAGS := gcc -g -c -std=c99
ADDITIONAL_INCLUDE := -I/home/lgxZJ/Libraries/CUnit/include
ADDITIONAL_LIB := -L/home/lgxZJ/Libraries/CUnit/lib

all.out : $(OBJECTS)
	gcc $(ADDITIONAL_LIB) $(ADDITIONAL_INCLUDE) -lcunit -coverage -o all.out $(OBJECTS)

typeChecker.o : typeChecker.c
	$(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) -fprofile-arcs -ftest-coverage $<

%.o : %.c
	$(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) $<

.PHONY : clean run retest show-coverage
clean:
	rm -f $(OBJECTS) all.out

run:
	./all.out

retest:
	make clean
	make
	./all.out

show-coverage:
	lcov --directory . --zerocounters
	./all.out
	lcov --directory . --capture --output-file coverage.info
	genhtml coverage.info
	firefox index.html      
