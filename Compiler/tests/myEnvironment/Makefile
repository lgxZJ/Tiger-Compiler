OUT_DIR := ./bin
OBJECTS := $(OUT_DIR)/test_myEnvironment.o    	$(OUT_DIR)/myEnvironment.o\
	$(OUT_DIR)/mySymbol.o 			$(OUT_DIR)/makeMemory.o \
	$(OUT_DIR)/typeMaker.o\
	$(OUT_DIR)/myTable.o		$(OUT_DIR)/abstractSyntaxMaker.o\
	$(OUT_DIR)/myTemp.o			$(OUT_DIR)/myTranslate.o\
	$(OUT_DIR)/myIRTree.o		$(OUT_DIR)/myFrame_X86.o\
	$(OUT_DIR)/lValueTreater.o	$(OUT_DIR)/mySemantic.o\
	$(OUT_DIR)/myError.o 		$(OUT_DIR)/breakGetter.o\
	$(OUT_DIR)/myEscape.o 		$(OUT_DIR)/breakChecker.o\
	$(OUT_DIR)/recursiveDecs.o 	$(OUT_DIR)/typeChecker.o\
	$(OUT_DIR)/forChecker.o 	$(OUT_DIR)/testHelper.o

COMPILE_FLAGS = gcc -g -c -std=c99
COVERAGE_FOLDER := ./coverage
COVERAGE_REPORT_FOLDER := ../../Coverage-Report

VPATH := ../../:../

tests.out: $(OUT_DIR) $(COVERAGE_FOLDER) $(OBJECTS)
	gcc -g -coverage -o $(OUT_DIR)/all.out $(OBJECTS) -lcunit

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(COVERAGE_FOLDER):
	mkdir -p $@


$(OUT_DIR)/myEnvironment.o : myEnvironment.c
	$(COMPILE_FLAGS) -fprofile-arcs -ftest-coverage -o $@ $<

$(OUT_DIR)/%.o : %.c
	$(COMPILE_FLAGS) -o $@ $<


.PHONY: clean run retest show-coverage
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
	lcov --directory . --capture --output-file $(COVERAGE_REPORT_FOLDER)/coverage-myEnvironment.info
	genhtml -f $(COVERAGE_REPORT_FOLDER)/coverage-myEnvironment.info --output-directory $(COVERAGE_FOLDER)

