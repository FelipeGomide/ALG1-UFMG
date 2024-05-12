SHELL := /bin/bash
CC = g++
CFLAGS = -Wall -ggdb

EXE = tp2.out
CPP = tp2.cpp

all: $(EXE)

tp2.out: $(CPP)
	$(CC) $(CPP) $(CFLAGS) -o tp2.out

run: $(EXE)
	./$(EXE)

test: $(exe)
	for file in tests/* ; do echo -e "\n\n$$file"; time ./tp2.out < $$file; done

testing: $(exe)
	for file in tests/* ; do \
		base=$$(basename "$$file");\
	       	name=$${base%.txt};\
	       	echo -e  "\n\n$$name" ;\
	       	./tp2.out < "$$file";\
	done

clean:
	rm -f tp2.out


