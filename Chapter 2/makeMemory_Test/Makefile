OBJECTS :=  Test_makeMemory.o makeMemory.o

VPATH := ../

COMPILE_FLAG := gcc -g -c

Test_makeMemory.out: $(OBJECTS)
	gcc -g -L/home/lgxZJ/Libraries/CUnit/lib -lcunit -o Test_makeMemory.out $(OBJECTS)

Test_makeMemory.o: Test_makeMemory.c makeMemory.h
	$(COMPILE_FLAG) -std=c99 -I/home/lgxZJ/Libraries/CUnit/include/ $^

makeMemory.o: makeMemory.c makeMemory.h
	$(COMPILE_FLAG) -std=c99 $^

.PHONY: clean
clean:
	rm -f Test_makeMemory.out $(OBJECTS)
