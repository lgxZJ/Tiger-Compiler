OUT_DIR := ./bin
OBJECTS := $(addprefix $(OUT_DIR)/, tests.o myRegisterAllocation.o\
                                    myAaiBase.o myLiveness.o myCFGraph.o\
                                    myDirectedGraph.o myMovable.o myComputable.o\
                                    myControlable.o myFrame_X86.o mySymbol.o\
                                    myTemp.o makeMemory.o myTable.o myIRTree.o\
                                    myTranslate.o myEnvironment.o lValueTreater.o\
                                    typeMaker.o mySemantic.o breakChecker.o breakGetter.o\
                                    myEscape.o myError.o myReport.o recursiveDecs.o\
                                    typeChecker.o abstractSyntaxMaker.o forChecker.o\
                                    lex.yy.o y.tab.o lexString.o stringEscape.o\
									myCanonical.o frontTester.o myMunch.o)

CXX_COMPILE_FLAGS := g++ -g -c -std=c++11
C_COMPILE_FLAGS := gcc -g -c -std=c99
LINK_FLAGS = g++ -g -static

VPATH := ../../:../../AAI
.RECIPEPREFIX := >

tests.out: $(OUT_DIR) $(OBJECTS)
> $(LINK_FLAGS) -o $(OUT_DIR)/all.out $(OBJECTS) -lcppunit

$(OUT_DIR):
> mkdir -p $(OUT_DIR)

$(OUT_DIR)/%.o : %.cpp
> $(CXX_COMPILE_FLAGS) -o $@ $<

$(OUT_DIR)/%.o : %.c
> $(C_COMPILE_FLAGS) -o $@ $<


.PHONY: clean run retest
clean:
>rm -f $(OBJECTS) $(OUT_DIR)/all.out

run:
> $(OUT_DIR)/all.out

retest:
> make clean
> make
> $(OUT_DIR)/all.out
