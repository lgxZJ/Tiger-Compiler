OUT_DIR := ./bin
OBJECTS := $(OUT_DIR)/tests.o   $(OUT_DIR)/myEnvironment.o\
    $(OUT_DIR)/mySymbol.o\
	$(OUT_DIR)/typeMaker.o		$(OUT_DIR)/makeMemory.o\
	$(OUT_DIR)/myTable.o		$(OUT_DIR)/abstractSyntaxMaker.o\
	$(OUT_DIR)/myError.o		$(OUT_DIR)/typeChecker.o\
	$(OUT_DIR)/testHelper.o		$(OUT_DIR)/recursiveDecs.o\
	$(OUT_DIR)/testFixture.o	$(OUT_DIR)/breakChecker.o\
	$(OUT_DIR)/myTranslate.o	$(OUT_DIR)/myFrame_X86.o\
	$(OUT_DIR)/myTemp.o			$(OUT_DIR)/myEscape.o\
	$(OUT_DIR)/forChecker.o     $(OUT_DIR)/y.tab.o\
    $(OUT_DIR)/lex.yy.o         $(OUT_DIR)/myReport.o\
    $(OUT_DIR)/lexString.o      $(OUT_DIR)/stringEscape.o\
	$(OUT_DIR)/myIRTree.o 		$(OUT_DIR)/breakGetter.o\
	$(OUT_DIR)/lValueTreater.o 

COMPILE_FLAGS := gcc -g -c -std=c99
COVERAGE_FOLDER := ./coverage

.RECIPEPREFIX := >
VPATH := ../../:../

all.out : $(OUT_DIR) $(OBJECTS)
> gcc -g -lcunit -o $(OUT_DIR)/all.out $(OBJECTS)

$(OUT_DIR):
> mkdir -p $@


$(OUT_DIR)/%.o : %.c
> $(COMPILE_FLAGS) -o $@ $<



.PHONY : clean retest run
clean:
> rm -f $(OBJECTS) $(OUT_DIR)/all.out

retest:
> make clean
> make
> $(OUT_DIR)/all.out

run:
> $(OUT_DIR)/all.out