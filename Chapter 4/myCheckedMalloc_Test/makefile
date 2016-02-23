myTest_CheckedMalloc.out: Test_myCheckedMalloc.o myCheckedMalloc.o
	cc -L../../../../../etc/cunit/lib -lcunit -o Test_myCheckedMalloc.out Test_myCheckedMalloc.o myCheckedMalloc.o

Test_myCheckedMalloc.o: Test_myCheckedMalloc.c 
	cc -I../../../../../etc/cunit/include -c Test_myCheckedMalloc.c

myCheckedMalloc.o: ../myCheckedMalloc.c ../myCheckedMalloc.h
	cc -c ../myCheckedMalloc.c

clean:
	rm -f myCheckedMalloc.o Test_myCheckedMalloc.out
