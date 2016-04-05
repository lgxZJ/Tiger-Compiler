Test_myTable.out: Test_myTable.o myTable.o makeMemory.o
	cc -g -L/home/lgxZJ/Libraries/CUnit/lib -lcunit -o Test_myTable.out Test_myTable.o myTable.o makeMemory.o

Test_myTable.o: Test_myTable.c ../myTable.h
	cc -g -std=c99 -I/home/lgxZJ/Libraries/CUnit/include -c Test_myTable.c

myTable.o: ../myTable.c ../myTable.h ../makeMemory.h
	cc -g -std=c99 -c ../myTable.c

makeMemory.o: ../makeMemory.c ../makeMemory.h
	cc -g -c -std=c99 ../makeMemory.c

clean:
	rm -f makeMemory.o myTable.o Test_myTable.o Test_myTable.out
