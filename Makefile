CC = gcc
CFLAGS = -Wall -g
DEBUG_CFLAGS = -Wall -g -DDEBUG

play: fileio.o game.o graph.o hw3.o random.o 
	$(CC) $(CFLAGS) fileio.o game.o graph.o hw3.o random.o -lhw3 -L. -o play

hw3.o: hw3.c hw3.h graph.h game.h random.h msg.h fileio.h 
	$(CC) $(CFLAGS) -c hw3.c -o hw3.o

graph.o: graph.c graph.h 
	$(CC) $(CFLAGS) -c graph.c -o graph.o

game.o: game.c game.h graph.h msg.h libhw3.h random.h 
	$(CC) $(CFLAGS) -c game.c -o game.o

fileio.o: fileio.c fileio.h game.h graph.h
	$(CC) $(CFLAGS) -c fileio.c -o fileio.o

random.o: random.c random.h
	$(CC) $(CFLAGS) -c random.c -o random.o

.PHONY: clean

clean:
	rm *.o play

test1:
	$(CC) $(DEBUG_CFLAGS) -c game.c -o game.o
	make play
	./play 1 < in.1 > myout.1
	diff myout.1 out.1 > diff.1

test2:
	$(CC) $(DEBUG_CFLAGS) -c game.c -o game.o
	make play
	./play 2 < in.2 > myout.2
	diff myout.2 out.2 > diff.2

tests: test1 test2