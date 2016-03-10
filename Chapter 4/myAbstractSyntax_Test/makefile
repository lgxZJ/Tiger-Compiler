OBJECTS = Test_myAbstractSyntax.o myAbstractSyntax.o mySymbol.o myTable.o myCheckedMalloc.o

Test_myAbstractSyntax.out: $(OBJECTS)
	cc -g -L/home/lgxZJ/Libraries/CUnit/lib -lcunit -o Test_myAbstractSyntax.out $(OBJECTS)

Test_myAbstractSyntax.o: Test_myAbstractSyntax.c ../myAbstractSyntax.h
	cc -g -std=c99 -I/home/lgxZJ/Libraries/CUnit/include -c Test_myAbstractSyntax.c

myAbstractSyntax.o: ../myAbstractSyntax.c ../myAbstractSyntax.h
	cc -c -g ../myAbstractSyntax.c

mySymbol.o: ../mySymbol.c ../mySymbol.h ../myCheckedMalloc.h ../myTable.h
	cc -c -std=c99 -g ../mySymbol.c

myTable.o: ../myTable.c ../myTable.h ../myCheckedMalloc.h
	cc -c -std=c99 -g ../myTable.c

myCheckedMalloc.o: ../myCheckedMalloc.c ../myCheckedMalloc.h
	cc -c -g ../myCheckedMalloc.c

clean:
	rm -f Test_myAbstractSyntax.out Test_mySbstractSyntax.o mySymbol.o myTable.o myCheckedMalloc.o
