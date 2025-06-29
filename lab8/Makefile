CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGETS = uppercase square_plus1

all: $(TARGETS)

uppercase: uppercase.c
	$(CC) $(CFLAGS) -o uppercase uppercase.c

square_plus1: square_plus1.c
	$(CC) $(CFLAGS) -o square_plus1 square_plus1.c

clean:
	rm -f $(TARGETS) input.txt