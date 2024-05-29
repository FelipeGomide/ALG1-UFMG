SHELL := /bin/bash
CC = g++
CFLAGS = -Wall -ggdb

EXE = tp3.out
CPP = tp3.cpp

all: $(EXE)

tp2.out: $(CPP)
	$(CC) $(CPP) $(CFLAGS) -o $(EXE)

run: $(EXE)
	./$(EXE)

test: $(exe)
	for file in tests/* ; do echo -e "\n\n$$file"; time ./$(EXE) < $$file; done

testing: $(exe)
	for file in tests/* ; do \
		base=$$(basename "$$file");\
	       	name=$${base%.txt};\
	       	echo -e  "\n\n$$name" ;\
	       	./$(EXE) < "$$file";\
	done

clean:
	rm -f $(EXE)


