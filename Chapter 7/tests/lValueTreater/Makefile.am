AM_CFLAGS = 	-std=c99

noinst_PROGRAMS = all.out
all_out_SOURCES = test_lValueTreater.c \
		../testHelper.c ../testHelper.h \
		../../makeMemory.c ../../makeMemory.h\
		../../myTable.c	../../mySymbol.c
all_out_LDADD = libLValueTreater.a -lcunit
all_out_LDFLAGS = -coverage -lcunit

noinst_LIBRARIES = libLValueTreater.a
libLValueTreater_a_SOURCES = ../../lValueTreater.c ../../lValueTreater.h
libLValueTreater_a_CFLAGS = -fprofile-arcs -ftest-coverage

clean:
	rm -f *.o
	rm -f *.out

run:
	./all.out

retest:
	make clean
	make
	make run

show-coverage:
	lcov --directory . --zerocounters
	./all.out
	mkdir -p coverage
	lcov --directory . --capture --output-file ./coverage/coverage.info
	genhtml -f ./coverage/coverage.info --output-directory ./coverage
	firefox ./coverage/index.html&

