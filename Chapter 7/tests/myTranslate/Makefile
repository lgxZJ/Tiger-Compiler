OUT_DIR := ./bin
OBJECTS := $(addprefix $(OUT_DIR)/, test_myTranslate.o myEnvironment.o 	mySymbol.o typeMaker.o makeMemory.o myTable.o abstractSyntaxMaker.o myError.o typeChecker.o testHelper.o recursiveDecs.o testFixture.o breakChecker.o myTranslate.o myFrame_X86.o myTemp.o myEscape.o forChecker.o y.tab.o lex.yy.o myReport.o lexString.o stringEscape.o myIRTree.o mySemantic.o debugPrint.o lValueTreater.o breakGetter.o)

COMPILE_FLAGS := gcc -g -c -std=c99
COVERAGE_OPTIONS := -fprofile-arcs -ftest-coverage
ADDITIONAL_INCLUDE := -I/home/lgxZJ/Libraries/CUnit/include
ADDITIONAL_LIB := -L/home/lgxZJ/Libraries/CUnit/lib
COVERAGE_FOLDER := ./coverage

.RECIPEPREFIX := >
VPATH := ../../:../

all.out : $(OBJECTS) | $(OUT_DIR) $(COVERAGE_FOLDER)
> gcc -g $(ADDITIONAL_INCLUDE) $(ADDITIONAL_LIB) -lcunit -coverage -o $(OUT_DIR)/all.out $(OBJECTS)

$(OUT_DIR):
> mkdir -p $@

$(COVERAGE_FOLDER):
> mkdir -p $@


$(OUT_DIR)/%.o : %.c
> $(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) -o $@ $<

$(OUT_DIR)/myTranslate.o : myTranslate.c
> $(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) $(COVERAGE_OPTIONS) -o $@ $<



.PHONY : clean retest run show-coverage
clean:
> rm -f $(OBJECTS) $(OUT_DIR)/all.out $(OUT_DIR)/*.gcda $(OUT_DIR)/*.gcno

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
