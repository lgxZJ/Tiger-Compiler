noinst_PROGRAMS = all.out
all_out_SOURCES = tests.cpp
all_out_LDADD = libmyCanonical.a

noinst_LIBRARIES = libmyCanonical.a
libmyCanonical_a_SOURCES = ../../myCanonical.cpp ../../myCanonical.h
libmyCanonical_a_CXXFLAGS = -fprofile-arcs -ftest-coverage

.RECIPEPREFIX = >

.PHNOY: clean
clean:
>   rm -r *.o *.out

retest:
>   make clean
>   make
>   make run

show-coverage:
>   lcov --directory . --zerocounters
>   ./all.out
>   mkdir -p coverage
>   lcov --directory . --capture --output-file ./coverage/coverage.info
>   genhtml -f ./coverage/coverage.info --output-directory ./coverage
>   firefox ./coverage/index.html&