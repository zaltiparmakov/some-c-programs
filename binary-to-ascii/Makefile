CC=gcc
FLAGS=-Wall -c
PROG=binarytoascii

all: $(PROG)

$(PROG): binarytoascii.o
	$(CC) binarytoascii.o -o $(PROG)

binarytoascii.o: binarytoascii.c
	$(CC) $(FLAGS) binarytoascii.c

clean:
	rm -rf $(PROG).o
