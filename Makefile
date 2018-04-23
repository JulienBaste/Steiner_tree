# Pathes
DSRC = src
DINC = inc
DOBJ = obj
DEXE = exe
DTST = tests
# Compiler flags
CFLAGS = -I$(DINC) -Wall -g
# Commands
CC = gcc $(CFLAGS)
# How to make Objects
$(DOBJ)/%.o: $(DSRC)/%.c
	$(CC) -o $@ -c $<

$(DOBJ)/%.o: $(DTST)/%.c
	$(CC) -o $@ -c $<
# Targets

all: naive test

# naive algo
naive: $(DOBJ)/amatrix.o $(DOBJ)/utils.o $(DOBJ)/uf.o $(DOBJ)/tGraph.o $(DOBJ)/nSteiner.o $(DOBJ)/naive.o
	$(CC) -o $(DEXE)/naive  $^
# tests
tGraphTest: $(DOBJ)/amatrix.o $(DOBJ)/utils.o $(DOBJ)/uf.o $(DOBJ)/tGraph.o $(DOBJ)/tGraphTest.o
	$(CC) -o $(DEXE)/tGraphTest $^ -lcunit
ufTest: $(DOBJ)/ufTest.o $(DOBJ)/utils.o $(DOBJ)/uf.o
	$(CC) -o $(DEXE)/ufTest $^ -lcunit
enumTest: $(DOBJ)/enumTest.o $(DOBJ)/amatrix.o $(DOBJ)/tGraph.o $(DOBJ)/utils.o $(DOBJ)/enum.o $(DOBJ)/uf.o
	$(CC) -o $(DEXE)/enumTest $^
test: $(DOBJ)/amatrix.o $(DOBJ)/tGraph.o $(DOBJ)/uf.o $(DOBJ)/utils.o $(DOBJ)/niceTD.o $(DOBJ)/enum.o $(DOBJ)/Steiner1.o $(DOBJ)/test.o
	$(CC) -o $(DEXE)/test $^


clean:
	rm $(DOBJ)/*
	rm $(DEXE)/*
