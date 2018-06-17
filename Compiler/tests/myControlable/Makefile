OUT_DIR := ./bin
OBJECTS := $(addprefix $(OUT_DIR)/, tests.o myCanonical.o myRegisterAllocation.o\
                                    myAaiBase.o myLiveness.o myCFGraph.o\
                                    myDirectedGraph.o myMovable.o myComputable.o\
                                    myControlable.o myFrame_X86.o mySymbol.o\
                                    myTemp.o makeMemory.o myTable.o myIRTree.o\
                                    myTranslate.o myEnvironment.o lValueTreater.o\
                                    typeMaker.o mySemantic.o breakChecker.o breakGetter.o\
                                    myEscape.o myError.o myReport.o recursiveDecs.o\
                                    typeChecker.o abstractSyntaxMaker.o forChecker.o\
                                    lex.yy.o y.tab.o lexString.o stringEscape.o)

CXX_COMPILE_FLAGS := g++ -g -c -std=c++11
C_COMPILE_FLAGS := gcc -g -c -std=c99
COVERAGE_OPTIONS := -fprofile-arcs -ftest-coverage
COVERAGE_FOLDER := ./coverage
COVERAGE_REPORT_FOLDER := ../../Coverage-Report

VPATH := ../../:../../AAI

all.out : $(COVERAGE_FOLDER) $(OBJECTS)
	g++ -g -coverage -o $(OUT_DIR)/all.out $(OBJECTS) -static -lcppunit -ldl

$(OBJECTS) : | $(OUT_DIR)

$(OUT_DIR):
	mkdir -p $@

$(COVERAGE_FOLDER):
	mkdir -p $@


$(OUT_DIR)/myControlable.o : myControlable.cpp
	$(CXX_COMPILE_FLAGS) $(COVERAGE_OPTIONS) -o $@ $<


$(OUT_DIR)/%.o : %.cpp
	$(CXX_COMPILE_FLAGS) -o $@ $<

$(OUT_DIR)/%.o : %.c
	$(C_COMPILE_FLAGS) -o $@ $<


.PHONY : clean retest run show-coverage
clean:
	rm -f $(OBJECTS) $(OUT_DIR)/all.out *.gcda *.gcno

retest:
	make clean
	make
	$(OUT_DIR)/all.out

run:
	$(OUT_DIR)/all.out

show-coverage:
	lcov --directory . --zerocounters
	$(OUT_DIR)/all.out
	lcov --directory . --capture --output-file $(COVERAGE_REPORT_FOLDER)/coverage-myControlable.info
	genhtml -f $(COVERAGE_REPORT_FOLDER)/coverage-myControlable.info --output-directory $(COVERAGE_FOLDER)
