OUT_DIR := ./bin
OBJECTS := $(OUT_DIR)/example.o     $(OUT_DIR)/abstractSyntaxMaker.o\
        $(OUT_DIR)/mySymbol.o       $(OUT_DIR)/myTable.o\
        $(OUT_DIR)/y.tab.o          $(OUT_DIR)/myReport.o\
        $(OUT_DIR)/do.o             \
        $(OUT_DIR)/lex.yy.o         $(OUT_DIR)/makeMemory.o\
        $(OUT_DIR)/stringEscape.o   $(OUT_DIR)/lexString.o

COMPILE_FLAGS := gcc -g -c -std=c99
LINK := gcc -g -std=c99

.RECIPEPREFIX := >
VPATH := ../:../../Lexical-Analysis:../../Commom

all.o : $(OUT_DIR) $(OBJECTS)
> $(LINK) -o $(OUT_DIR)/all.out $(OBJECTS)

$(OUT_DIR):
> mkdir -p $(OUT_DIR)


$(OUT_DIR)/%.o : %.c
> $(COMPILE_FLAGS) -o $@ $<



.PHONY: clean run

clean:
> rm -f $(OUT_DIR)/all.out $(OBJECTS)

run:
> $(OUT_DIR)/all.out
