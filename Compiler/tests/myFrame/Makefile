OUT_DIR := ./bin
OBJECTS := $(OUT_DIR)/myFrame_X86.o		$(OUT_DIR)/test_myFrame_X86.o\
	$(OUT_DIR)/testHelper.o				$(OUT_DIR)/makeMemory.o\
	$(OUT_DIR)/myTemp.o					$(OUT_DIR)/mySymbol.o\
	$(OUT_DIR)/myTable.o				$(OUT_DIR)/myIRTree.o\
	$(OUT_DIR)/lValueTreater.o

COMPILE_FLAGS := gcc -g -c -std=c99
COVERAGE_OPTIONS := -fprofile-arcs -ftest-coverage
COVERAGE_FOLDER := ./coverage
COVERAGE_REPORT_FOLDER := ../../Coverage-Report

.RECIPEPREFIX := >
VPATH := ../../:../

all.out : $(OUT_DIR) $(COVERAGE_FOLDER) $(OBJECTS)
> gcc -g -lcunit -coverage -o $(OUT_DIR)/all.out $(OBJECTS)

$(OUT_DIR):
> mkdir -p $@

$(COVERAGE_FOLDER):
> mkdir -p $@


$(OUT_DIR)/myFrame_X86.o : myFrame_X86.c
> $(COMPILE_FLAGS) $(COVERAGE_OPTIONS) -o $@ $<

$(OUT_DIR)/%.o : %.c
> $(COMPILE_FLAGS) -o $@ $<


.PHONY : clean retest run show-coverage
clean:
> rm -f $(OBJECTS) $(OUT_DIR)/all.out

retest:
> make clean
> make
> $(OUT_DIR)/all.out

run:
> $(OUT_DIR)/all.out

show-coverage:
> lcov --directory . --zerocounters
> $(OUT_DIR)/all.out
> lcov --directory . --capture --output-file $(COVERAGE_REPORT_FOLDER)/coverage-myFrame.info
> genhtml -f $(COVERAGE_REPORT_FOLDER)/coverage-myFrame.info --output-directory $(COVERAGE_FOLDER)



