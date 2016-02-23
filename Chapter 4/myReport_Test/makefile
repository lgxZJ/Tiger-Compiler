Test_myReport.out: Test_myReport.o myReport.o
	cc -g -L/home/gaoxin/etc/cunit/lib -lcunit -o Test_myReport.out Test_myReport.o myReport.o

Test_myReport.o: Test_myReport.c ../myReport.h
	cc -g -I/home/gaoxin/etc/cunit/include -c Test_myReport.c

myReport.o: ../myReport.c ../myReport.h
	cc -g -c ../myReport.c

clean:
	rm -f Test_myReport.o myReport.o
