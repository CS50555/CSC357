CC = gcc
CFLAGS = -g

all : factorial fibonacci

factorial : factorial_gdb_practice.o
	$(CC) $(CFLAGS) -o factorial factorial_gdb_practice.o

fibonacci : fibonacci_gdb_practice.o
	$(CC) $(CFLAGS) -o fibonacci fibonacci_gdb_practice.o

factorial.o : factorial_gdb_practice.c
	$(CC) $(CFLAGS) -c factorial_gdb_practice.c

fibonacci.o : fibonacci_gdb_practice.c
	$(CC) $(CFLAGS) -c fibonacci_gdb_practice.c

clean :
	-rm *.o
	-rm factorial
	-rm fibonacci