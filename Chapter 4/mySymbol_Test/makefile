Test_mySymbol.out: Test_mySymbol.o myTable.o mySymbol.o myCheckedMalloc.o
	cc -g -L/home/gaoxin/etc/cunit/lib -lcunit -o Test_mySymbol.out Test_mySymbol.o mySymbol.o myCheckedMalloc.o myTable.o

Test_mySymbol.o: Test_mySymbol.c ../mySymbol.h
	cc -g -I/home/gaoxin/etc/cunit/include -c Test_mySymbol.c

myTable.o: ../myTable.c ../myTable.h ../myCheckedMalloc.h
	cc -g -std=c99 -c ../myTable.c

mySymbol.o: ../mySymbol.c ../myCheckedMalloc.h ../myTable.h
	cc -g -std=c99 -c ../mySymbol.c

myCheckedMalloc.o: ../myCheckedMalloc.c ../myCheckedMalloc.h
	cc -g -c ../myCheckedMalloc.c

clean:
	rm -f myCheckedMalloc.o myTable.o Test_mySymbol.o Test_mySymbol.out
