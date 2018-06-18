B0;136;0cB0;136;0cOUT_DIR := ./bin
OBJECTS := $(OUT_DIR)/forChecker.o      $(OUT_DIR)/mySymbol.o\
            $(OUT_DIR)/myTable.o        $(OUT_DIR)/test_forChecker.o\
            $(OUT_DIR)/testHelper.o     $(OUT_DIR)/makeMemory.o

COMPILE_FLAGS := gcc -g -c -std=c99
COVERAGE_OPTIONS := -fprofile-arcs -ftest-coverage
COVERAGE_FOLDER := ./coverage
COVERAGE_REPORT_FOLDER := ../../Coverage-Report

VPATH := ../../:../

all.out : $(OUT_DIR) $(COVERAGE_FOLDER) $(OBJECTS)
	gcc -g -coverage -o $(OUT_DIR)/all.out $(OBJECTS) -lcunit

$(OUT_DIR):
	mkdir -p $@

$(COVERAGE_FOLDER):
	mkdir -p $@


$(OUT_DIR)/%.o : %.c
	$(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) -o $@ $<

$(OUT_DIR)/forChecker.o : forChecker.c
	$(COMPILE_FLAGS) $(COVERAGE_OPTIONS) -o $@ $<



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
	lcov --directory . --capture --output-file $(COVERAGE_REPORT_FOLDER)/coverage-forChecker.info
	genhtml -f $(COVERAGE_REPORT_FOLDER)/coverage-forChecker.info --output-directory $(COVERAGE_FOLDER)


