OUT_DIR := ./bin
OBJECTS := $(OUT_DIR)/test_myEscape.o       $(OUT_DIR)/myEscape.o\
            $(OUT_DIR)/mySymbol.o           $(OUT_DIR)/testHelper.o\
            $(OUT_DIR)/myTable.o            $(OUT_DIR)/makeMemory.o\
            $(OUT_DIR)/testFixture.o        $(OUT_DIR)/abstractSyntaxMaker.o\
            $(OUT_DIR)/typeMaker.o          $(OUT_DIR)/myEnvironment.o\
            $(OUT_DIR)/myTemp.o             $(OUT_DIR)/myTranslate.o\
            $(OUT_DIR)/myFrame_X86.o        $(OUT_DIR)/typeChecker.o

COMPILE_FLAGS := gcc -g -c -std=c99
LINK := gcc -g

COVERAGE_OPTIONS := -fprofile-arcs -ftest-coverage
ADDITIONAL_INCLUDE := -I/home/lgxZJ/Libraries/CUnit/include
ADDITIONAL_LIB := -L/home/lgxZJ/Libraries/CUnit/lib
COVERAGE_FOLDER := ./coverage

.RECIPEPREFIX := >
VPATH := ../../:../

all : $(OUT_DIR) $(COVERAGE_FOLDER) $(OBJECTS)
> $(LINK) $(ADDITIONAL_LIB) -lcunit -coverage -o $(OUT_DIR)/all.out $(OBJECTS)

$(OUT_DIR):
> mkdir -p $@

$(COVERAGE_FOLDER):
> mkdir -p $@


$(OUT_DIR)/myEscape.o : myEscape.c
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


