Exercise.out: program.o util.o introduce.o
	cc -o Exercise.out program.o util.o introduce.o

program.o: program.c introduce.h
	cc -c program.c

util.o: util.c util.h
	cc -c util.c

introduce.o: introduce.c introduce.h
	cc -c introduce.c

clean:
	rm -f Exercise.out program.o util.o introduce.o
