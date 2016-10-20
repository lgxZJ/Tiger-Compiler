OUT_DIR := ./bin
OBJECTS := $(OUT_DIR)/test_breakChecker.o $(OUT_DIR)/breakChecker.o\
	$(OUT_DIR)/testHelper.o

COMPILE_FLAGS := gcc -g -c -std=c99
ADDITIONAL_INCLUDE := -I/home/lgxZJ/Libraries/CUnit/include
ADDITIONAL_LIB := -L/home/lgxZJ/Libraries/CUnit/lib
COVERAGE_OPTIONS := -fprofile-arcs -ftest-coverage

VPATH := ../../:../


all.out : $(OBJECTS)
	gcc -g $(ADDITIONAL_LIB) -lcunit -coverage -o $(OUT_DIR)/all.out $(OBJECTS)

$(OUT_DIR)/breakChecker.o : breakChecker.c
	$(COMPILE_FLAGS)  $(ADDITIONAL_INCLUDE) $(COVERAGE_OPTIONS) -o $@ $<

$(OUT_DIR)/%.o : %.c
	$(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) -o $@ $<



.PHONY : clean retest run show-coverage

clean:
	rm -f $(OBJECTS) $(OUT_DIR)/all.out

retest:
	make clean
	make
	$(OUT_DIR)/all.out

run:
	$(OUT_DIR)/all.out

show-coverage:
	lcov --directory . --zerocounters
	$(OUT_DIR)/all.out
	lcov --directory . --capture --output-file coverage.info
	genhtml -f coverage.info
	firefox index.html &
