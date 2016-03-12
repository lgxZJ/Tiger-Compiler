Exercise.out : y.tab.o lex.yy.o myReport.o
	cc -g -o Exercise.out y.tab.o lex.yy.o myReport.o

y.tab.o : y.tab.c 
	cc -g -std=c99 -c y.tab.c

y.tab.c : myTiger.y
	yacc -d -v myTiger.y

y.tab.h : y.tab.c
	echo "y.tab.h are created with y.tab.c\n"

lex.yy.o : lex.yy.c y.tab.h myReport.h
	cc -g -c lex.yy.c

lex.yy.c : myTiger.lex
	lex myTiger.lex

myReport.o : myReport.c myReport.h y.tab.h
	cc -g -c myReport.c

clean :
	 rm -f myReport.o lex.yy.c lex.yy.o y.tab.h y.tab.c  y.tab.o Exercise
