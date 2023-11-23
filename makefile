NAME=schedualer_SecVer

CFILES=$(NAME).c $(NAME)_test.c Task.c uid.c vector_third.c heap.c pq_SecVer.c
OBJECTS=$(NAME).o $(NAME)_test.o Task.o uid.o vector_third.o heap.o pq_SecVer.o

BINARY=$(NAME).out

VPATH=src:test
LDFLAGS=-I. -Iinclude

BIN_GD_DIR=./bin/debug/
BIN_GC_DIR=./bin/release/

CC=gcc
GD_DEBUGFLAG=-ansi -pedantic-errors -Wall -Wextra -g
GC_RELEASEFLAG=-ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3



all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

$(OBJECTS): $(CFILES)
	$(CC) $(GD_DEBUGFLAG) -c $^ $(LDFLAGS)

debug: $(CFILES)
	$(CC) $(GD_DEBUGFLAG) -o $(BINARY) $(CFILES) $(LDFLAGS)

release: $(CFILES)
	$(CC) $(GC_RELEASEFLAG) -o $(BINARY) $(CFILES)  $(LDFLAGS)
	
clean:
	rm $(BINARY) $(OBJECTS)
