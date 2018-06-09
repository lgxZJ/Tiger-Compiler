OUT_DIR := ./bin
OBJECTS := $(OUT_DIR)/test_myEscape.o\
            $(OUT_DIR)/mySymbol.o 		$(OUT_DIR)/makeMemory.o \
	        $(OUT_DIR)/typeMaker.o      $(OUT_DIR)/myEnvironment.o\
	        $(OUT_DIR)/myTable.o		$(OUT_DIR)/abstractSyntaxMaker.o\
	        $(OUT_DIR)/myTemp.o			$(OUT_DIR)/myTranslate.o\
	        $(OUT_DIR)/myIRTree.o		$(OUT_DIR)/myFrame_X86.o\
	        $(OUT_DIR)/lValueTreater.o\
	        $(OUT_DIR)/myError.o 		$(OUT_DIR)/breakGetter.o\
	        $(OUT_DIR)/myEscape.o 		$(OUT_DIR)/breakChecker.o\
	        $(OUT_DIR)/typeChecker.o\
	        $(OUT_DIR)/forChecker.o 	$(OUT_DIR)/testHelper.o\
            $(OUT_DIR)/testFixture.o

COMPILE_FLAGS := gcc -g -c -std=c99
LINK := gcc -g

COVERAGE_OPTIONS := -fprofile-arcs -ftest-coverage
COVERAGE_FOLDER := ./coverage
COVERAGE_REPORT_FOLDER := ../../Coverage-Report

.RECIPEPREFIX := >
VPATH := ../../:../

all : $(OUT_DIR) $(COVERAGE_FOLDER) $(OBJECTS)
> $(LINK) -lcunit -coverage -o $(OUT_DIR)/all.out $(OBJECTS)

$(OUT_DIR):
> mkdir -p $@

$(COVERAGE_FOLDER):
> mkdir -p $@


$(OUT_DIR)/myEscape.o : myEscape.c
> $(COMPILE_FLAGS) $(COVERAGE_OPTIONS) -o $@ $<

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
> lcov --directory . --capture --output-file $(COVERAGE_REPORT_FOLDER)/coverage-myEscape.info
> genhtml -f $(COVERAGE_REPORT_FOLDER)/coverage-myEscape.info --output-directory $(COVERAGE_FOLDER)



