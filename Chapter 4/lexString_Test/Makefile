OBJECTS = Test_lexString.o lexString.o

VPATH = ../

#	define directie, the same as COMPILE
#define COMPILE = 
#gcc -g
#endef

COMPILE = gcc -g

Test_lexString.out: $(OBJECTS)
	gcc -g -L/home/lgxZJ/Libraries/CUnit/lib -lcunit -o Test_lexString.out $(OBJECTS)

Test_lexString.o: Test_lexString.c lexString.h
	$(COMPILE) -std=c99 -I/home/lgxZJ/Libraries/CUnit/include -c $^

lexString.o: lexString.c lexString.h
	$(COMPILE) -c $^

.PHONY: clean
clean:
	rm -f Test_lexString.out $(OBJECTS)
