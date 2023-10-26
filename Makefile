# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -Wpedantic -std=c99 -O0 -g

# define targets
TARGETS=mk

#define object-files
OBJ=mk.o

build: $(TARGETS)

mk:
	$(CC) $(CFLAGS) mk.c aux.c structs.c -o mk

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
pack:
	zip -FSr zip.zip README.md Makefile *.c *.h

clean:
	rm -f $(TARGETS) $(OBJ)

.PHONY: pack clean
