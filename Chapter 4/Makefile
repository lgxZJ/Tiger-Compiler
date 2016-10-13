OBJECTS = myPrintAbstractSyntax.o myAbstractSyntax.o mySymbol.o\
	myTable.o myReport.o makeMemory.o  y.tab.o lex.yy.o\
	lexString.o stringEscape.o


Exercise.out: $(OBJECTS)
	cc -g -o Exercise.out $(OBJECTS)

myPrintAbstractSyntax.o: myPrintAbstractSyntax.c myPrintAbstractSyntax.h
	cc -g -std=c99 -c myPrintAbstractSyntax.c

myAbstractSyntax.o: myAbstractSyntax.c myAbstractSyntax.h
	cc -g -c myAbstractSyntax.c

mySymbol.o: mySymbol.c mySymbol.h
	cc -g -std=c99 -c mySymbol.c

myTable.o: myTable.c myTable.h
	cc -g -std=c99 -c myTable.c

myReport.out: myReport.c myReport.h y.tab.h 
	cc -g -c myReport.c

makeMemory.o: makeMemory.c makeMemory.h
	cc -g -c -std=c99  makeMemory.c

y.tab.o: y.tab.c
	cc -std=c99 -g -c y.tab.c

y.tab.c: myTiger.y
	yacc -dv myTiger.y

y.tab.h: y.tab.c
	echo "y.tab.h was created at the same time as y.tab.c"

lex.yy.c: myTiger.lex
	lex myTiger.lex

lexString.o: lexString.c lexString.h
	cc -g -c lexString.c

stringEscape.o: stringEscape.c stringEscape.h
	cc -g -c -std=c99  stringEscape.c

.PHNOY: clean
clean:
	rm -f Exercise.out $(OBJECTS)
