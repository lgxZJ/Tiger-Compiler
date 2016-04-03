OBJECTS := Test_stringEscape.o stringEscape.o makeMemory.o

VPATH := ../

EXTRA_LIB_PATH := /home/lgxZJ/Libraries/CUnit/lib
EXTRA_INCLUDE_PATH := /home/lgxZJ/Libraries/CUnit/include

Test_stringEscape.out: $(OBJECTS)
	gcc -g -L$(EXTRA_LIB_PATH) -lcunit -o Test_stringEscape.out $(OBJECTS)

Test_stringEscape.o: Test_stringEscape.c stringEscape.h
	gcc -g  -I$(EXTRA_INCLUDE_PATH) -c -std=c99 $^

stringEscape.o: stringEscape.c stringEscape.h
	gcc -g -std=c99 -c $^

makeMemory.o: makeMemory.c makeMemory.h
	gcc -g -c -std=c99 $^

.PHONY: clean
clean:
	rm -f Test_stringEscape.out $(OBJECTS)
