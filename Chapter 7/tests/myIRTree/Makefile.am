AM_CFLAGS = -I/home/lgxZJ/Libraries/CUnit/include\
    -L/home/lgxZJ/Libraries/CUnit/lib       -std=c99

noinst_PROGRAMS = all.out
all_out_SOURCES = test_myIRTree.c		../../mySymbol.c\
	../../myTable.c     	../../makeMemory.c\
    ../testHelper.c         ../../myTemp.c
all_out_LDFLAGS = -lcunit -coverage
all_out_LDADD = libmyIRTree.a

noinst_LIBRARIES = libmyIRTree.a
libmyIRTree_a_SOURCES = ../../myIRTree.c ../../myIRTree.h
libmyIRTree_a_CFLAGS = -fprofile-arcs -ftest-coverage 


run:
	./all.out

show-coverage:
	lcov --directory . --zerocounters
	./all.out
	mkdir -p coverage
	lcov --directory . --capture --output-file ./coverage/coverage.info
	genhtml -f ./coverage/coverage.info --output-directory ./coverage
	firefox ./coverage/index.html&
