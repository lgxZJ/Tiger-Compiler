OUT_DIR := ./bin
OBJECTS := $(OUT_DIR)/test_myEnvironment.o    	$(OUT_DIR)/myEnvironment.o\
	$(OUT_DIR)/mySymbol.o 			$(OUT_DIR)/makeMemory.o \
	$(OUT_DIR)/typeMaker.o 			$(OUT_DIR)/myTable.o\
	$(OUT_DIR)/testHelper.o			$(OUT_DIR)/myTemp.o\
	$(OUT_DIR)/myTranslate.o		$(OUT_DIR)/myFrame_X86.o\
	$(OUT_DIR)/myIRTree.o

ADDITION_LIB := -L/home/lgxZJ/Libraries/CUnit/lib
ADDITION_INCLUDE := -I/home/lgxZJ/Libraries/CUnit/include
COMPILE_FLAGS = gcc -g -c -std=c99
COVERAGE_FOLDER := ./coverage

VPATH := ../../:../
.RECIPEPREFIX := >

tests.out: $(OUT_DIR) $(COVERAGE_FOLDER) $(OBJECTS)
> gcc -g $(ADDITION_LIB) -lcunit -coverage -o $(OUT_DIR)/all.out $(OBJECTS)

$(OUT_DIR):
> mkdir -p $(OUT_DIR)

$(COVERAGE_FOLDER):
> mkdir -p $@


$(OUT_DIR)/myEnvironment.o : myEnvironment.c
> $(COMPILE_FLAGS) $(ADDITION_INCLUDE) -fprofile-arcs -ftest-coverage -o $@ $<

$(OUT_DIR)/%.o : %.c
> $(COMPILE_FLAGS) $(ADDITION_INCLUDE) -o $@ $<


.PHONY: clean run retest show-coverage
clean:
>rm -f $(OBJECTS) $(OUT_DIR)/all.out

run:
> $(OUT_DIR)/all.out

retest:
> make clean
> make
> $(OUT_DIR)/all.out

show-coverage:
> lcov --directory . --zerocounters
> $(OUT_DIR)/all.out
> lcov --directory . --capture --output-file $(COVERAGE_FOLDER)/coverage.info
> genhtml -f $(COVERAGE_FOLDER)/coverage.info --output-directory $(COVERAGE_FOLDER)
> firefox $(COVERAGE_FOLDER)/index.html &
