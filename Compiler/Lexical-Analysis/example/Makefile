OUT_DIR := ./bin
OBJECTS := $(OUT_DIR)/example.o $(OUT_DIR)/do.o $(OUT_DIR)/myReport.o \
	$(OUT_DIR)/stringEscape.o  $(OUT_DIR)/makeMemory.o\
	$(OUT_DIR)/lex.yy.o

COMPILE_FLAGS := gcc -g -c -std=c99
VPATH := ../../Commom:../

all : $(OBJECTS)
	gcc -g $(OBJECTS) -o $(OUT_DIR)/all.out

$(OUT_DIR)/%.o : %.c
	$(COMPILE_FLAGS) -o $@ $<


.PHONY : clean run

clean:
	rm -f $(OBJECTS) $(OUT_DIR)/all.out

run:
	$(OUT_DIR)/all.out
