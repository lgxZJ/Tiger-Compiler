OUT_DIR := ./bin
OBJECTS := $(OUT_DIR)/test_recursiveDecs.o	$(OUT_DIR)/mySemantic.o\
	$(OUT_DIR)/myEnvironment.o		$(OUT_DIR)/mySymbol.o\
	$(OUT_DIR)/typeMaker.o			$(OUT_DIR)/makeMemory.o\
	$(OUT_DIR)/myTable.o			$(OUT_DIR)/abstractSyntaxMaker.o\
	$(OUT_DIR)/myError.o			$(OUT_DIR)/typeChecker.o\
	$(OUT_DIR)/testHelper.o			$(OUT_DIR)/recursiveDecs.o\
	$(OUT_DIR)/testFixture.o		$(OUT_DIR)/breakChecker.o\
	$(OUT_DIR)/myTemp.o			$(OUT_DIR)/myTranslate.o\
	$(OUT_DIR)/myFrame_X86.o		$(OUT_DIR)/myEscape.o\
	$(OUT_DIR)/forChecker.o			$(OUT_DIR)/myIRTree.o\
	$(OUT_DIR)/breakGetter.o		$(OUT_DIR)/lValueTreater.o

COVERAGE_FOLDER := ./coverage
COMPILE_FLAGS := gcc -g -c -std=c99

VPATH := ../../:../

all : $(OUT_DIR) $(COVERAGE_FOLDER) $(OBJECTS)
	gcc -g -coverage -o $(OUT_DIR)/all.out $(OBJECTS) -lcunit

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(COVERAGE_FOLDER):
	mkdir -p $@

# make rules
$(OUT_DIR)/%.o : %.c
	$(COMPILE_FLAGS) $(ADDITIONAL_INCLUDE) -o $@ $<
$(OUT_DIR)/recursiveDecs.o : recursiveDecs.c
	$(COMPILE_FLAGS) -fprofile-arcs -ftest-coverage $(ADDITIONAL_INCLUDE) -o $@ $<

.PHONY: clean run show-coverage rebuild
clean:
	rm -f $(OBJECTS) $(OUT_DIR)/all.out

run:
	$(OUT_DIR)/all.out

show-coverage:
	lcov --directory . --zerocounters
	$(OUT_DIR)/all.out
	lcov --directory . --capture --output-file $(COVERAGE_FOLDER)/code-coverage.info
	genhtml -f $(COVERAGE_FOLDER)/code-coverage.info --output-directory $(COVERAGE_FOLDER)


retest:
	make clean
	make
	$(OUT_DIR)/all.out
