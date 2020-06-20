CC=gcc209
CFLAGS=-D_BSD_SOURCE -D_GNU_SOURCE
CFLAGS=-g -D_DEFAULT_SOURCE

ish: main.o dfa.o dynarray.o pipe.o makecommand.o
	$(CC) $(CFLAGS) main.o dfa.o dynarray.o pipe.o makecommand.o -o ish

main.o: main.c dfa.h dynarray.h makecommand.h pipe.h
	$(CC) $(CFLAGS) -c main.c

dfa.o: dfa.c dfa.h dynarray.h
	$(CC) $(CFLAGS) -c dfa.c

dynarray.o: dynarray.c dynarray.h
	$(CC) $(CFLAGS) -c dynarray.c

pipe.o: pipe.c pipe.h
	$(CC) $(CFLAGS) -c pipe.c

makecommand.o: makecommand.c makecommand.h
	$(CC) $(CFLAGS) -c makecommand.c

clean:
	rm -f ./ish