OUT_DIR := ./bin
OBJECTS := $(OUT_DIR)/test_typeChecker.o	$(OUT_DIR)/typeChecker.o\
	$(OUT_DIR)/typeMaker.o			$(OUT_DIR)/makeMemory.o\
	$(OUT_DIR)/mySymbol.o			$(OUT_DIR)/testHelper.o\
	$(OUT_DIR)/myTable.o

VPATH := ../../:../

COMPILE_FLAGS := gcc -g -c -std=c99
COVERAGE_FOLDER := ./coverage
COVERAGE_REPORT_FOLDER := ../../Coverage-Report


all.out : $(OUT_DIR) $(COVERAGE_FOLDER) $(OBJECTS)
	gcc -lcunit -coverage -o $(OUT_DIR)/all.out $(OBJECTS)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(COVERAGE_FOLDER):
	mkdir -p $@

$(COVERAGE_REPORT_FOLDER):
	mkdir -p $@


$(OUT_DIR)/typeChecker.o : typeChecker.c
	$(COMPILE_FLAGS) -fprofile-arcs -ftest-coverage -o $@ $<

$(OUT_DIR)/%.o : %.c
	$(COMPILE_FLAGS) -o $@ $<

.PHONY : clean run retest show-coverage
clean:
	rm -f $(OBJECTS) $(OUT_DIR)/all.out

run:
	$(OUT_DIR)/all.out

retest:
	make clean
	make
	$(OUT_DIR)/all.out

show-coverage:
	lcov --directory . --zerocounters
	$(OUT_DIR)/all.out
	lcov --directory . --capture --output-file $(COVERAGE_REPORT_FOLDER)/coverage-typeChecker.info
	genhtml -f $(COVERAGE_REPORT_FOLDER)/coverage-typeChecker.info --output-directory $(COVERAGE_FOLDER)

