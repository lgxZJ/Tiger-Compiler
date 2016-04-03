Test_mySymbol.out: Test_mySymbol.o myTable.o mySymbol.o makeMemory.o
	cc -g -L/home/lgxZJ/Libraries/CUnit/lib -lcunit -o Test_mySymbol.out Test_mySymbol.o mySymbol.o makeMemory.o myTable.o

Test_mySymbol.o: Test_mySymbol.c ../mySymbol.h
	cc -g -I/home/lgxZJ/Libraries/CUnit/include -c Test_mySymbol.c

myTable.o: ../myTable.c ../myTable.h ../myCheckedMalloc.h
	cc -g -std=c99 -c ../myTable.c

mySymbol.o: ../mySymbol.c ../myCheckedMalloc.h ../myTable.h
	cc -g -std=c99 -c ../mySymbol.c

makeMemory.o: ../makeMemory.c ../makeMemory.h
	cc -g -std=c99 -c $^

clean:
	rm -f makeMemory.o myTable.o Test_mySymbol.o Test_mySymbol.out
