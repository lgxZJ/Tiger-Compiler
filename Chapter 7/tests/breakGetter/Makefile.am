AM_CFLAGS  = -I/home/lgxZJ/Libraries/CUnit/lib \
                -L/home/lgxZH/Libraries/CUnit/include \
                -std=c99

noinst_PROGRAMS = all.out
all_out_SOURCES = test_breakGetter.c ../testHelper.c ../testHelper.h ../../makeMemory.c
all_out_LDFLAGS = -lcunit -coverage
all_out_LDADD = libbreakGetter.a

noinst_LIBRARIES = libbreakGetter.a
libbreakGetter_a_SOURCES = ../../breakGetter.c ../../breakGetter.h
libbreakGetter_a_CFLAGS = -fprofile-arcs -ftest-coverage

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