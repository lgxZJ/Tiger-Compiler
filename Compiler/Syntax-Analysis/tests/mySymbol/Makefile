OUT_DIR := ./bin
OBJECTS := $(OUT_DIR)/test_mySymbol.o	$(OUT_DIR)/makeMemory.o\
	$(OUT_DIR)/mySymbol.o		$(OUT_DIR)/myTable.o\
	$(OUT_DIR)/testHelper.o

COMPILE_FLAGS := gcc -g -c -std=c99
ADDITIONAL_INCLUDE := -I/home/lgxZJ/Libraries/CUnit/include
ADDITIONAL_LIB := -L/home/lgxZJ/Libraries/CUnit/lib
COVERAGE_OPTIONS := -fprofile-arcs -ftest-coverage
COVERAGE_REPORT_FOLDER := ./coverage
COVERAGE_FILE_FOLDER := ../../../Coverage-Report

VPATH := ../../:../:../../../Commom:../../../TestHelper

all.out : $(OUT_DIR) $(OBJECTS)
	gcc $(ADDITIONAL_INCLUDE) $(ADDITIONAL_LIB) -lcunit -coverage -o $(OUT_DIR)/all.out $(OBJECTS)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(OUT_DIR)/mySymbol.o : mySymbol.c
	$(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) $(COVERAGE_OPTIONS) -o $@ $<

$(OUT_DIR)/%.o : %.c
	$(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) -o $@ $<



.PHONY: clean run retest show-coverage

clean:
	rm -f $(OBJECTS) $(OUT_DIR)/all.out *.gcda *.gcno

run:
	$(OUT_DIR)/all.out

retest:
	make clean
	make
	$(OUT_DIR)/all.out

show-coverage:
	lcov --directory . --zerocounters
	$(OUT_DIR)/all.out
	lcov --directory . --capture --output-file $(COVERAGE_FILE_FOLDER)/coverage-mySymbol.info
	genhtml -f $(COVERAGE_FILE_FOLDER)/coverage-mySymbol.info --output-directory $(COVERAGE_REPORT_FOLDER)
	firefox $(COVERAGE_REPORT_FOLDER)/index.html &
