OUT_DIR := ./bin
OBJECTS := $(OUT_DIR)/test_myTable.o    	$(OUT_DIR)/myEnvironment.o\
	$(OUT_DIR)/mySymbol.o 			$(OUT_DIR)/makeMemory.o \
	$(OUT_DIR)/typeMaker.o			$(OUT_DIR)/y.tab.o\
	$(OUT_DIR)/lex.yy.o				$(OUT_DIR)/lexString.o\
	$(OUT_DIR)/myTable.o		$(OUT_DIR)/abstractSyntaxMaker.o\
	$(OUT_DIR)/myTemp.o			$(OUT_DIR)/myTranslate.o\
	$(OUT_DIR)/myIRTree.o		$(OUT_DIR)/myFrame_X86.o\
	$(OUT_DIR)/lValueTreater.o	$(OUT_DIR)/mySemantic.o\
	$(OUT_DIR)/myError.o 		$(OUT_DIR)/breakGetter.o\
	$(OUT_DIR)/myEscape.o 		$(OUT_DIR)/breakChecker.o\
	$(OUT_DIR)/recursiveDecs.o 	$(OUT_DIR)/typeChecker.o\
	$(OUT_DIR)/forChecker.o 	$(OUT_DIR)/testHelper.o\
	$(OUT_DIR)/myReport.o		$(OUT_DIR)/stringEscape.o

COMPILE_FLAGS = gcc -g -c -std=c99
COVERAGE_FOLDER := ./coverage
COVERAGE_REPORT_FOLDER := ../../Coverage-Report

VPATH := ../../:../
.RECIPEPREFIX := >

tests.out: $(OUT_DIR) $(COVERAGE_FOLDER) $(OBJECTS)
> gcc -g -lcunit -coverage -o $(OUT_DIR)/all.out $(OBJECTS)

$(OUT_DIR):
> mkdir -p $(OUT_DIR)

$(COVERAGE_FOLDER):
> mkdir -p $@


$(OUT_DIR)/myTable.o : myTable.c
> $(COMPILE_FLAGS) -fprofile-arcs -ftest-coverage -o $@ $<

$(OUT_DIR)/%.o : %.c
> $(COMPILE_FLAGS) -o $@ $<


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
> lcov --directory . --capture --output-file $(COVERAGE_REPORT_FOLDER)/coverage-myTable.info
> genhtml -f $(COVERAGE_REPORT_FOLDER)/coverage-myTable.info --output-directory $(COVERAGE_FOLDER)
> firefox $(COVERAGE_FOLDER)/index.html &
