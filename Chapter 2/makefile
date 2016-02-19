Exercise.out: myDriver.o myReport.o myTiger.o
	cc -g -o Exercise.out myDriver.o myReport.o myTiger.o

myDriver.o: myDriver.c myTokens.h myReport.h
	cc -g -c myDriver.c

myReport.o: myReport.c myReport.h
	cc -g -c myReport.c

myTiger.o: lex.yy.c myTokens.h myReport.h
	cc -g -c -o myTiger.o lex.yy.c

lex.yy.c: myTiger.lex
	lex myTiger.lex

clean:
	rm -f lex.yy.c myTiger.o myReport.o myDriver.o Exercise.out
