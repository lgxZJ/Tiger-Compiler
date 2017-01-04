OUT_DIR = ./Libraries
OBJECTS = $(addprefix $(OUT_DIR)/,\
            abstractSyntaxMaker.o     breakChecker.o      breakGetter.o\
            forChecker.o            lex.yy.o            lexString.o\
            lValueTreater.o         makeMemory.o        myEnvironment.o\
            myError.o               myEscape.o          myFrame_X86.o\
            myIRTree.o              myReport.o          mySemantic.o\
            mySymbol.o              myTable.o           myTemp.o\
            myTranslate.o           recursiveDecs.o     typeChecker.o\
            typeMaker.o             y.tab.o		stringEscape.o\
	        frontTester.o)

.RECIPEPREFIX = >
COMPILE = gcc -c -g -std=c99


all : $(OBJECTS)
> ar rcs $(OUT_DIR)/frontend.a $(OBJECTS)
> rm -f $(OUT_DIR)/*.o


$(OBJECTS): | $(OUT_DIR)

$(OUT_DIR):
> mkdir -p $@


$(OUT_DIR)/%.o : %.c
> $(COMPILE) -o $@ $<



.PHONY: clean
clean:
> rm -f $(OBJECTS) $(OUT_DIR)/frontend.a
