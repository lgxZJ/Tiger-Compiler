OBJECTS := example.o do.o myReport.o stringEscape.o makeMemory.o lex.yy.o

COMPILE_FLAGS := gcc -g -c -std=c99

VPATH := ../Commom

all.out : $(OBJECTS)
	gcc -g $(OBJECTS) -o output/all.out

%.o : %.c
	$(COMPILE_FLAGS) $<

.PHONY : clean run

clean:
	rm -f $(OBJECTS) all.out

run:
	./all.out
