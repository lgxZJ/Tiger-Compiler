OUT_DIR := ./bin
OBJECTS := $(OUT_DIR)/test_breakChecker.o $(OUT_DIR)/breakChecker.o\
	$(OUT_DIR)/testHelper.o

COMPILE_FLAGS := gcc -g -c -std=c99
COVERAGE_OPTIONS := -fprofile-arcs -ftest-coverage
COVERAGE_FOLDER := ./coverage
COVERAGE_REPORT_FOLDER := ../../Coverage-Report
VPATH := ../../:../


all.out : $(OUT_DIR) $(OBJECTS)
	gcc -g -coverage -o $(OUT_DIR)/all.out $(OBJECTS) -lcunit

$(OUT_DIR):
	mkdir -p $@

$(OUT_DIR)/breakChecker.o : breakChecker.c
	$(COMPILE_FLAGS) $(COVERAGE_OPTIONS) -o $@ $<

$(OUT_DIR)/%.o : %.c
	$(COMPILE_FLAGS) -o $@ $<



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
	lcov --directory . --capture --output-file $(COVERAGE_REPORT_FOLDER)/coverage-breakChecker.info
	genhtml -f $(COVERAGE_REPORT_FOLDER)/coverage-breakChecker.info --output-directory $(COVERAGE_FOLDER)

