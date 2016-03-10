OBJECTS = Test_myPrintAbstractSyntax.o myPrintAbstractSyntax.o \
		myAbstractSyntax.o mySymbol.o myTable.o \
		myCheckedMalloc.o

vpath %.c ../
vpath %.h ../

Test_myPrintAbstractSyntax.out: $(OBJECTS)
	cc -g -L/home/lgxZJ/Libraries/CUnit/lib -o Test_myPrintAbstractSyntax.out $(OBJECTS) -lcunit

Test_myPrintAbstractSyntax.o: Test_myPrintAbstractSyntax.c myAbstractSyntax.h
	cc -g -I/home/lgxZJ/Libraries/CUnit/include -c -std=c99 $<

myPrintAbstractSyntax.o: myPrintAbstractSyntax.c myAbstractSyntax.h
	cc -g -std=c99 -c $<

myAbstractSyntax.o: myAbstractSyntax.c myAbstractSyntax.h
	cc -g -c $<

mySymbol.o: mySymbol.c mySymbol.h myCheckedMalloc.h myTable.h
	cc -g -std=c99 -c $<

myTable.o: myTable.c myTable.h myCheckedMalloc.h
	cc -g -std=c99 -c $<

myCheckedMalloc.o: myCheckedMalloc.c myCheckedMalloc.h
	cc -g -c $<


.PHNOY: clean
clean:
	rm -f Test_myPrintAbstractSyntax.out Test_myPrintAbstractSyntax.o myPrintAbstractSyntax.o myAbstractSyntax.o mySymbol.o myTable.o myCheckedMalloc.o
