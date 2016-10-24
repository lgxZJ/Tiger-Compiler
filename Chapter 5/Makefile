OUT_DIR := ./bin
OBJECTS = $(OUT_DIR)/y.tab.o			$(OUT_DIR)/main.o\
	$(OUT_DIR)/abstractSyntaxMaker.o	$(OUT_DIR)/mySymbol.o\
	$(OUT_DIR)/myTable.o			$(OUT_DIR)/myReport.o\
	$(OUT_DIR)/makeMemory.o			$(OUT_DIR)/lex.yy.o\
	$(OUT_DIR)/lexString.o			$(OUT_DIR)/stringEscape.o\
	$(OUT_DIR)/myEnvironment.o		$(OUT_DIR)/mySemantic.o\
	$(OUT_DIR)/typeMaker.o			$(OUT_DIR)/myError.o\
	$(OUT_DIR)/typeChecker.o		$(OUT_DIR)/recursiveDecs.o\
	$(OUT_DIR)/breakChecker.o

COMPILE_FLAGS := gcc -g -c -std=c99

all.out: $(OUT_DIR) $(OBJECTS)
	gcc -g -o $(OUT_DIR)/all.out $(OBJECTS)

y.tab.c: myTiger.y
	yacc -dv myTiger.y

y.tab.h: y.tab.c
	echo "y.tab.h was created at the same time as y.tab.c"

lex.yy.c: myTiger.lex
	lex myTiger.lex	

$(OUT_DIR)/%.o : %.c
	$(COMPILE_FLAGS) -o $@ $<




.PHNOY: clean run
clean:
	rm -f $(OUT_DIR)/all.out $(OBJECTS) y.tab.c y.tab.h lex.yy.c

run:
	$(OUT_DIR)/all.out
