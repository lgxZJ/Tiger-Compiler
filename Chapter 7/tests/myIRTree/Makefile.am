AM_CFLAGS = -std=c99 -static

noinst_PROGRAMS = all.out
all_out_SOURCES = test_myIRTree.c		../../mySymbol.c\
	../../myTable.c     	../../makeMemory.c\
    ../testHelper.c         ../../myTemp.c\
	../../lValueTreater.c	../../myFrame_X86.c
all_out_LDFLAGS = -lcunit -coverage
all_out_LDADD = libmyIRTree.a -lcunit	

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
