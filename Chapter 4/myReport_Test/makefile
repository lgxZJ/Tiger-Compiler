LIB_PATH := /home/lgxZJ/Libraries/CUnit/lib
INCLUDE_PATH := /home/lgxZJ/Libraries/CUnit/include

Test_myReport.out: Test_myReport.o myReport.o
	cc -g -L$(LIB_PATH) -lcunit -o Test_myReport.out Test_myReport.o myReport.o

Test_myReport.o: Test_myReport.c ../myReport.h
	cc -g -I$(INCLUDE_PATH) -c Test_myReport.c

myReport.o: ../myReport.c ../myReport.h
	cc -g -c ../myReport.c

clean:
	rm -f Test_myReport.o myReport.o
