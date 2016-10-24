OUT_DIR := ./bin
OBJECTS := $(OUT_DIR)/test_myTable.o	$(OUT_DIR)/testHelper.o\
	$(OUT_DIR)/makeMemory.o		$(OUT_DIR)/myTable.o

COMPILE_FLAGS := gcc -g -c -std=c99
COVERAGE_OPTIONS := -fprofile-arcs -ftest-coverage
ADDITIONAL_INCLUDE := -I/home/lgxZJ/Libraries/CUnit/include
ADDITIONAL_LIB := -L/home/lgxZJ/Libraries/CUnit/lib
COVERAGE_REPORT_FOLDER := ./coverage
COVERAGE_FILE_FOLDER := ../../../Coverage-Report

VPATH := ../../:../:../../../TestHelper:../../../Commom

all.out : $(OUT_DIR) $(OBJECTS)
	gcc -g $(ADDITIONAL_INCLUDE) $(ADDITIONAL_LIB) -lcunit -coverage -o $(OUT_DIR)/all.out $(OBJECTS)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)


$(OUT_DIR)/%.o : %.c
	$(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) -o $@ $<

$(OUT_DIR)/myTable.o : myTable.c
	$(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) $(COVERAGE_OPTIONS) -o $@ $<



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
	lcov --directory . --capture --output-file $(COVERAGE_FILE_FOLDER)/coverage-myTable.info
	genhtml -f $(COVERAGE_FILE_FOLDER)/coverage-myTable.info --output-directory $(COVERAGE_REPORT_FOLDER)
	firefox $(COVERAGE_REPORT_FOLDER)/index.html &


