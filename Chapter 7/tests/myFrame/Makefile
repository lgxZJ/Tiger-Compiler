OUT_DIR := ./bin
OBJECTS := $(OUT_DIR)/myFrame_X86.o		$(OUT_DIR)/test_myFrame_X86.o\
	$(OUT_DIR)/testHelper.o				$(OUT_DIR)/makeMemory.o\
	$(OUT_DIR)/myTemp.o					$(OUT_DIR)/mySymbol.o\
	$(OUT_DIR)/myTable.o			$(OUT_DIR)/myIRTree.o\
	$(OUT_DIR)/lValueTreater.o

COMPILE_FLAGS := gcc -g -c -std=c99
COVERAGE_OPTIONS := -fprofile-arcs -ftest-coverage
ADDITIONAL_INCLUDE := -I/home/lgxZJ/Libraries/CUnit/include
ADDITIONAL_LIB := -L/home/lgxZJ/Libraries/CUnit/lib
COVERAGE_FOLDER := ./coverage

.RECIPEPREFIX := >
VPATH := ../../:../

all.out : $(OUT_DIR) $(COVERAGE_FOLDER) $(OBJECTS)
> gcc -g $(ADDITIONAL_LIB) -lcunit -coverage -o $(OUT_DIR)/all.out $(OBJECTS)

$(OUT_DIR):
> mkdir -p $@

$(COVERAGE_FOLDER):
> mkdir -p $@


$(OUT_DIR)/myFrame_X86.o : myFrame_X86.c
> $(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) $(COVERAGE_OPTIONS) -o $@ $<

$(OUT_DIR)/%.o : %.c
> $(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) -o $@ $<


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
> lcov --directory . --capture --output-file $(COVERAGE_FOLDER)/coverage.info
> genhtml -f $(COVERAGE_FOLDER)/coverage.info --output-directory $(COVERAGE_FOLDER)
> firefox $(COVERAGE_FOLDER)/index.html &


