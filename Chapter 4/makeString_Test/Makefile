OBJECTS = Test_makeString.o makeString.o

VPATH = ../

#	define directie, the same as COMPILE
#define COMPILE = 
#gcc -g
#endef

COMPILE = gcc -g

Test_makeString.out: $(OBJECTS)
	gcc -g -L/home/lgxZJ/Libraries/CUnit/lib -lcunit -o Test_makeString.out $(OBJECTS)

Test_makeString.o: Test_makeString.c makeString.h
	$(COMPILE) -std=c99 -I/home/lgxZJ/Libraries/CUnit/include -c $^

makeString.o: makeString.c makeString.h
	$(COMPILE) -c $^

.PHONY: clean
clean:
	rm -f Test_makeString.out Test_makeString.o makeString.o
