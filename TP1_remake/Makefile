SHELL := /bin/bash
CC = g++
CFLAGS = -pg -g -O3

EXE = tp1.out
CPP = tp1.cpp

all: $(EXE)

tp1.out: $(CPP)
	$(CC) $(CPP) $(CFLAGS) -o tp1.out

run: $(EXE)
	./$(EXE)

test: $(EXE)
	for file in tests/* ; do printf "\n\n"; time ./tp1.out < $$file; done

clean:
	rm -f tp1.out